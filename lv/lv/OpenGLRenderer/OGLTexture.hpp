// *********************************************************************
//  OGLTexture   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_OGLTEXTURE_HPP
#define LV_OGLTEXTURE_HPP

#include <GL/GLU.h>

#include <lv/Graphics/Texture.hpp>

namespace lv
{
	class OGLTexture : public Texture
	{
		GLuint	texture_;

	public:
		OGLTexture();
		~OGLTexture();

	};
}



#endif // LV_OGLTEXTURE_HPP