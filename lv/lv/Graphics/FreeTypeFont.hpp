#if 0


// *********************************************************************
//  FreeTypeFont   version:  1.0   ¡¤  date: 09/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GLYPHCACHE_HPP
#define LV_GLYPHCACHE_HPP

#include <list>

#include <boost/unordered_map.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <lv/Graphics/RenderFwd.hpp>
#include <lv/Buffer.hpp>
#include <lv/LRUList.hpp>
#include <lv/Graphics/Font.hpp>
#include <lv/Graphics/Rect.hpp>
#include <lv/Graphics/PixelFormat.hpp>


namespace lv
{
	class FreeTypeFont : public Font
	{
	protected:

		TexturePtr	texture_;
		Size	tex_size_;		// size of the texture (to be created)

		typedef GIL::ViewT<PF_L8>::type	view_t;

		Point	next_glyph_pos_;	// Where to put the next glyph (if the texture is not yet full)

		typedef LRUList<wchar_t, boost::fast_pool_allocator<wchar_t> > lru_list;

		struct Glyph
		{
			Rect	rect;
			Point	offset;
			int		advance;
			lru_list::iterator lru_it;
		};

		typedef boost::unordered_map<wchar_t, Glyph, boost::hash<wchar_t>, std::equal_to<wchar_t>, 
			boost::pool_allocator<std::pair<wchar_t, Glyph> > >	glyph_map;
		glyph_map	glyphs_;

		lru_list	lru_glyphs_;

		BufferPtr	buf_holder_;	// hold a reference to the buffer so that it won't be deallocated

		static	FT_Library	ft_lib_;
		static	int			ft_ref_count_;		// reference count

		FT_Face	face_;

		bool	anti_aliased_;


	public:

		/**
		 * @param name identifier of this font
		 * @param height height of the font in pixels
		 * @param anti_aliased anti-aliased using 256 levels of opacity.
		 * @param tex_size size of the texture to be created to cache the glyphs. If it's beyond the 
		 *	capacity of the video card, the maximum possible value will be used.
		 */
		FreeTypeFont(std::string const & name, size_t height, bool anti_aliased = true, Size tex_size = Size(1024));
		~FreeTypeFont();

		/**
		 * Clear the current cache and load the font from buffer
		 * @param buffer data of the font file. If it's loaded successfully, the buffer will be kept 
		 *	until the destruction of this object or another call to @a load.
		 * @exception std::runtime_error
		 */
		void	load(BufferPtr buffer);

		/**
		 * Clear the current cache and load the font from the given file.
		 * @exception std::runtime_error
		 */
		void	load(std::string const & file);


		/**
		 * @copydoc Font::loaded()
		 */
		bool	loaded() const override;

		/// TODO : clipping
		void	draw_char(wchar_t c, Point const & pt, ColorRect const & colors) override;

		/**
		 * @copydoc Font::advance()
		 */
		int		advance(wchar_t c) override;

	protected:

		void	clear();

		void	load_impl(BufferPtr buffer, std::string const & file);

		void	update_cache(std::wstring const & txt) override;

		void	update_cache(wchar_t c);

		/**
		 * @exception std::runtime_error if ret is NOT zero
		 */
		void	check_ft_result(int ret, std::string const & err_str);

		/// maximum number of glyphs that can be cached in the texture.
		size_t	cache_capacity() const;

		/// 
		Size	slot_size() const;

		void	draw_glyph_to_view(view_t & view);
	};
}

#endif //LV_GLYPHCACHE_HPP


#endif