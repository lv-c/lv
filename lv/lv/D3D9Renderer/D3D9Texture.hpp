// *********************************************************************
//  D3D9Texture   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_D3D9TEXTURE_HPP
#define LV_D3D9TEXTURE_HPP

#include <lv/Graphics/Texture.hpp>
#include <d3d9.h>

namespace lv
{
	class D3D9Texture : public Texture
	{
		LPDIRECT3DTEXTURE9	texture_;

	public:
		D3D9Texture();
		
		/**
		 * @see Texture::load(ConstBufferRef buffer)
		 * @exception std::runtime_error on failure
		 */
		virtual	void	load(ConstBufferRef buffer);


		LPDIRECT3DTEXTURE9	d3d_texture() const
		{
			return texture_;
		}
	};
}


#endif // LV_D3D9TEXTURE_HPP