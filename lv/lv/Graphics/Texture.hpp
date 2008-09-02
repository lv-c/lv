// *********************************************************************
//  Texture   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_TEXTURE_HPP
#define LV_TEXTURE_HPP

#include <lv/Buffer.hpp>
#include <lv/Graphics/Size.hpp>

namespace lv
{
	class Texture
	{
	protected:
		Size	image_size_;
		Size	texture_size_;

	public:


		enum PixelFormat
		{

		};

		Size	image_size() const
		{
			return image_size_;
		}

		Size	texture_size() const
		{
			return texture_size_;
		}
		
		/**
		 * load the texture from a file in memory.
		 * @exception std::runtime_error on failure
		 */
		virtual	void	load(ConstBufferRef buffer) = 0;
	};
}


#endif