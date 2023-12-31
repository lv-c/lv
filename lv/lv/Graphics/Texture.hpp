// *********************************************************************
//  Texture   version:  1.0   ��  date: 08/05/2008
//  --------------------------------------------------------------------
//		�����Ļ��ࡣ
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/assert.hpp>

#include <lv/Buffer.hpp>
#include <lv/Ensure.hpp>
#include <lv/Graphics/Rect.hpp>
#include <lv/Graphics/PixelFormat.hpp>

#include <memory>


namespace lv
{

	enum TextureMapAccess
	{
		TMA_READ_ONLY,
		TMA_WRITE_ONLY,
		TMA_READ_WRITE
	};

		
	class Texture
	{
		template<class T>
		class AutoUnmap
		{
			Texture & tex_;
		public:
			AutoUnmap(Texture & tex)
				: tex_(tex)
			{
			}

			void operator () (T * p)
			{
				delete p;
				tex_.unmap_impl();
			}
		};

	protected:
		Size	image_size_;
		Size	texture_size_;

		PixelFormat	format_;

	public:

		/**
		 * load the texture from a file in memory.
		 * @exception std::runtime_error on failure
		 */
		virtual	void	load(ConstBufferRef buffer) = 0;


		/**
		 * use this meta-function to deduce the result type of function @a map.
		 */
		template<int PF>
		struct ViewPtrT
		{
			using type = std::shared_ptr<typename GIL::ViewT<PF>::type>;
		};

		/**
		 * Map the given area of this texture to a gil view. 
		 * @param PF compile-time pixel format of this texture. It should be the same as the 
		 *	runtime format or an exception will be thrown.
		 * @return a smart pointer of the gil view. On final release, the smart pointer will
		 *	automatically unmap the texture.
		 * @note DO NOT hold the result long.
		 * @exception std::runtime_error if @a PF is not same as @a this->format() or @a rect 
		 *	is out of bound.
		 */
		template<int PF>
		typename ViewPtrT<PF>::type map(Rect const & rect)
		{
			LV_ENSURE(PF == this->format_, "Texture::map Invalid format");
			LV_ENSURE(Rect(Point(), texture_size_).contains(rect), "Texture::map rect out of bound");

			using view_t = typename GIL::ViewT<PF>::type;
			return ViewPtrT<PF>::type(new view_t(boost::gil::interleaved_view(rect.width(), rect.height(), 
				(GIL::XIteratorT<PF>::type) map_impl(rect), PixelBytes<PF>::value * texture_size_.cx)), 
				AutoUnmap<view_t>(*this));
		}


		Size	image_size() const
		{
			return image_size_;
		}

		Size	texture_size() const
		{
			return texture_size_;
		}

		PixelFormat	format() const
		{
			return format_;
		}
		
		

	private:
		
		virtual	void *	map_impl(Rect const & rect) = 0;
		virtual	void 	unmap_impl() = 0;

	};
}
