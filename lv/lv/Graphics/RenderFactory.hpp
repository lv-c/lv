// *********************************************************************
//  RenderFactory   version:  1.0   ¡¤  date: 09/03/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/RenderFwd.hpp>

#include <lv/Singleton.hpp>
#include <lv/Graphics/Texture.hpp>

namespace lv
{
	class RenderFactory : public Singleton<RenderFactory>
	{
	public:

		virtual TexturePtr	create_texture(Size size, PixelFormat format) = 0;
	};
}