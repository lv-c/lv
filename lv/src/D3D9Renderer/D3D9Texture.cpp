#include <lv/D3D9Renderer/D3D9Texture.hpp>
#include <lv/D3D9Renderer/D3D9Renderer.hpp>
#include <d3dx9tex.h>
#include <lv/Exception.hpp>

namespace lv
{

	D3D9Texture::D3D9Texture()
		: texture_(NULL)
	{
	}

	void D3D9Texture::load(ConstBufferRef buffer)
	{
		D3D9Renderer & renderer = static_cast<D3D9Renderer &>(Renderer::instance()); 
		D3DXIMAGE_INFO info;
		
		DX_TIF( D3DXCreateTextureFromFileInMemoryEx(renderer.device(), buffer.data(), buffer.size(), D3DX_DEFAULT,
			D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &info, 
			NULL, &texture_) );

		// get the size of the image and the texture
		image_size_.set(info.Width, info.Height);

		D3DSURFACE_DESC desc;
		HRESULT hr = texture_->GetLevelDesc(0, &desc);
		if(FAILED(hr))
			texture_size_ = image_size_;	// set it to the image's size
		else
			texture_size_.set(desc.Width, desc.Height);
	}
}