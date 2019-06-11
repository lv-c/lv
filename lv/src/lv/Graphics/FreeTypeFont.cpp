#if 0


#include <lv/Graphics/FreeTypeFont.hpp>
#include <lv/Graphics/RenderFactory.hpp>
#include <lv/Graphics/Renderer.hpp>
#include <lv/Ensure.hpp>

#include <boost/assert.hpp>


namespace lv
{
	FT_Library FreeTypeFont::ft_lib_ = nullptr;
	int FreeTypeFont::ft_ref_count_ = 0;

// number of pixels between glyphs
#define GLYPH_PAD_SPACE	2


	FreeTypeFont::FreeTypeFont(std::string const & name, size_t height, bool anti_aliased /* = true */, Size tex_size /* = Size */)
		: Font(name, height)
		, tex_size_(tex_size)
		, face_(nullptr)
		, anti_aliased_(anti_aliased)
	{
		if (ft_ref_count_ ++ == 0)	
		{
			FT_Init_FreeType(&ft_lib_);
		}
	}

	FreeTypeFont::~FreeTypeFont()
	{
		clear();

		if (--ft_ref_count_ == 0)
		{
			FT_Done_FreeType(ft_lib_);
		}
	}

	void FreeTypeFont::clear()
	{
		if (face_ != nullptr)
		{
			FT_Done_Face(face_);
			face_ = nullptr;
		}

		texture_.reset();
		buf_holder_.reset();
		glyphs_.clear();
		lru_glyphs_.clear();

		next_glyph_pos_ = Point();
	}

	void FreeTypeFont::load(BufferPtr buffer)
	{
		BOOST_ASSERT(buffer && !buffer->empty());
		load_impl(buffer, std::string());
	}

	void FreeTypeFont::load(std::string const & file)
	{
		load_impl(BufferPtr(), file);
	}

	void FreeTypeFont::load_impl(BufferPtr buffer, std::string const & file)
	{
		clear();

		//
		int ret = 0;
		if (buffer)
		{
			ret = FT_New_Memory_Face(ft_lib_, reinterpret_cast<FT_Byte*>(&(*buffer)[0]), buffer->size(), 0, &face_);
		}
		else
		{
			ret = FT_New_Face(ft_lib_, file.c_str(), 0, &face_);
		}

		check_ft_result(ret, std::string("FreeTypeFont::load Error loading font : ") + name());
		

		// check the character map
		if (face_->charmap == nullptr)
		{
			FT_Done_Face(face_);
			face_ = nullptr;
			throw std::runtime_error(std::string("FreeTypeFont::load The font contains no character map : ") + name());
		}

		// create the texture
		Size sz = Renderer::instance().max_texture_size();
		tex_size_.set(std::min(sz.cx, tex_size_.cx), std::min(sz.cy, tex_size_.cy));
		texture_ = RenderFactory::instance().create_texture(tex_size_, PF_L8);

		LV_ENSURE(texture_, "FreeTypeFont::load Error creating texture: " + name());

		// keep the buffer
		buf_holder_ = buffer;
	}

	void FreeTypeFont::update_cache(std::wstring const & txt)
	{
		for (wchar_t c : txt)
		{
			update_cache(c);
		}
	}


	void FreeTypeFont::update_cache(wchar_t c)
	{
		size_t capacity = cache_capacity();

		glyph_map::iterator it = glyphs_.find(c);
		if (it != glyphs_.end())
		{
			it->second.lru_it = lru_glyphs_.touch(it->second.lru_it);
			return;
		}


		Point pt_glyph;
		bool clear = false;	// need to clear the old glyph first

		if (glyphs_.size() >= capacity)	// there's no free space in the texture
		{
			wchar_t lru_c = lru_glyphs_.erase_last();

			Glyph g = glyphs_[lru_c];
			pt_glyph = g.rect.pos();
			glyphs_.erase(lru_c);

			clear = true;	
		}
		else
		{
			pt_glyph = next_glyph_pos_;
		}

		Size sz_slot = slot_size();

		Rect clip(pt_glyph, sz_slot);

		int const format = PF_L8;
		Texture::ViewPtrT<format>::type view_ptr = texture_->map<format>(clip);

		if (clear)
		{
			boost::gil::fill_pixels(*view_ptr, GIL::PixelT<format>::type(0));
		}

		// render the glyph
		Glyph glyph;
		Size sz_glyph;
		if (FT_Load_Char(face_, c, FT_LOAD_RENDER | (anti_aliased_ ? FT_LOAD_TARGET_NORMAL :
			FT_LOAD_TARGET_MONO)) != 0)
		{
			glyph.rect = Rect(pt_glyph, Size(1));
			glyph.advance = 0;
		}
		else
		{
			draw_glyph_to_view(*view_ptr);
			glyph.rect = Rect(pt_glyph, Size(std::min<int32_t>(face_->glyph->bitmap.width, sz_slot.cx),
				std::min<int32_t>(face_->glyph->bitmap.rows, sz_slot.cy)));
			glyph.advance = face_->glyph->advance.x >> 6;
			glyph.offset.set(face_->glyph->metrics.horiBearingX >> 6, face_->glyph->metrics.horiBearingY >> 6);
		} 

		glyph.lru_it = lru_glyphs_.add(c);
		glyphs_[c] = glyph;
	}


	int FreeTypeFont::advance(wchar_t c)
	{
		update_cache(c);
		glyph_map::iterator it = glyphs_.find(c);

		if (it != glyphs_.end())
		{
			return it->second.advance;
		}
		else
		{
			BOOST_ASSERT(false);
			return 0;
		}
	}

	bool FreeTypeFont::loaded() const
	{
		return face_ != nullptr;
	}

	size_t FreeTypeFont::cache_capacity() const
	{
		Size sz = slot_size();

		return (tex_size_.cx / sz.cx) * (tex_size_.cy / sz.cy);
	}

	Size FreeTypeFont::slot_size() const
	{
		return Size(height() + GLYPH_PAD_SPACE);
	}

	void FreeTypeFont::draw_glyph_to_view(view_t & view)
	{
		FT_Bitmap & bmp = face_->glyph->bitmap;

		Size sz_slot(slot_size());
		Size sz_draw(std::min<int32_t>(sz_slot.cx, bmp.width), std::min<int32_t>(sz_slot.cy, bmp.rows));

		for (int i = 0; i < bmp.rows; ++i)
		{
			unsigned char * src = bmp.buffer + i * bmp.pitch;
			
			view_t::x_iterator dest_it = view.row_begin(i);

			switch (bmp.pixel_mode)
			{
			case FT_PIXEL_MODE_GRAY:
				std::copy(src, src + sz_draw.cx, dest_it);
				break;
			case FT_PIXEL_MODE_MONO:
				for (int j = 0; j < sz_draw.cx; ++j)
					*dest_it ++ = (src [j / 8] & (0x80 >> (j & 7))) ? 255 : 0;
				break;
			default:
				BOOST_ASSERT(false && "unsupported pixel mode");
				break;
			}
		}
	}
}



#endif