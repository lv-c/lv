#pragma warning(push)
#pragma warning(disable : 4244) //'initializing' : conversion from 'const lv::int32' to 'float', possible loss of data

#include <lv/D3D9Renderer/D3D9Renderer.hpp>
#include <lv/D3D9Renderer/D3D9Texture.hpp>
#include <lv/Graphics/Rect.hpp>
#include <lv/Graphics/ColorRect.hpp>
#include <lv/Exception.hpp>
#include <boost/assert.hpp>

namespace lv
{

	uint32 const D3D9Renderer::VERTEX_FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	D3D9Renderer::D3D9Renderer(LPDIRECT3DDEVICE9 device)
		: device_(device)
		, vbuffer_(NULL)
	{
	}

	/*
	void D3D9Renderer::init(Size const & size, bool windowed, int bit_depth)
	{
		d3d_ = Direct3DCreate9(D3D_SDK_VERSION);
		if(d3d_ == NULL)
			throw(std::runtime_error("error creating direct3d"));

		D3DDISPLAYMODE d3ddm;
		DX_TIF(d3d_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm));
		D3DPRESENT_PARAMETERS d3dparm;
		ZeroMemory(d3dparm, sizeof(d3dparm));

		d3dparm.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dparm.Windowed = windowed;
		d3dparm.BackBufferWidth = size.cx;
		d3dparm.BackBufferHeight = size.cy;

		
	}
	*/

	void D3D9Renderer::begin_scene()
	{
		BOOST_ASSERT(device_ != NULL);
		DX_TIF(device_->BeginScene());

		device_->SetStreamSource(0, vbuffer_, 0, sizeof(CustomVertex));
		device_->SetFVF(VERTEX_FVF);

		device_->SetRenderState(D3DRS_ZENABLE, FALSE);
		device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


		// texture
		device_->SetTexture(0, NULL);
		cur_texture_ = NULL;

		// state
		device_->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		device_->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		device_->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		device_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		device_->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		device_->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

		cur_blend_ = BL_MODULATE;
	}


	void D3D9Renderer::end_scene()
	{
		device_->EndScene();
	}

	void D3D9Renderer::draw_quad(Point const & dest_pos, Texture const & tex, Rect const & src_rect, ColorRect const & colors 
		/* = ColorRect::white */, Blend blend /* = BL_MODULATE */, Size const & dest_size /* = Size() */)
	{
		CustomVertex vertices[4];

		// colors
		for(int i = 0; i < 4; ++i)
			vertices[i].color = colors[i].argb();

		
		Rect dest_rect(dest_pos, dest_size == Size() ? src_rect.size() : dest_size);


		// top-left vertex
		vertices[0].dest_pt = dest_rect.top_left();
		vertices[0].tex_pt	= src_rect.top_left();

		// top-right vertex
		vertices[1].dest_pt = dest_rect.top_right();
		vertices[1].tex_pt	= src_rect.top_right();

		// bottom-left vertex
		vertices[2].dest_pt = dest_rect.bottom_left();
		vertices[2].tex_pt	= src_rect.bottom_left();

		// bottom-right vertex
		vertices[3].dest_pt = dest_rect.bottom_right();
		vertices[3].tex_pt	= src_rect.bottom_right();


		// size of the texture
		Size tex_size = tex.texture_size();
		for(int i = 0; i < 4; ++i)
		{
			vertices[i].tex_pt.x /= tex_size.cx;
			vertices[i].tex_pt.y /= tex_size.cy;
		}


		// set the texture if the texture passed in is not the same as the current texture
		set_texture(static_cast<D3D9Texture const*>(&tex)->d3d_texture());

		// blend
		if(blend != cur_blend_)
		{
			cur_blend_ = blend;
			device_->SetTextureStageState( 0, D3DTSS_COLOROP, map_blend(blend));
		}


		void * buf = NULL;
		if(SUCCEEDED(vbuffer_->Lock(0, sizeof(vertices), &buf, 0)))
		{
			memcpy(buf, vertices, sizeof(vertices));
			vbuffer_->Unlock();
		
			device_->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	void D3D9Renderer::frame_quad(Rect const & rect, bool fill, ColorRect const & colors)
	{
		CustomVertex vertices[5];

		// colors
		for(int i = 0; i < 4; ++i)
			vertices[i].color = colors[i].argb();
	
		if(! fill)
		{
			vertices[2].color = colors[3].argb();
			vertices[3].color = colors[2].argb();
			vertices[4].color = vertices[0].color;
		}

		// 
		vertices[0].dest_pt = rect.top_left();
		vertices[1].dest_pt = rect.top_right();

		if(fill)
		{
			vertices[2].dest_pt = rect.bottom_left();
			vertices[3].dest_pt = rect.bottom_right();
		}
		else
		{
			vertices[2].dest_pt = rect.bottom_right();
			vertices[3].dest_pt = rect.bottom_left();
			vertices[4].dest_pt = vertices[0].dest_pt;
		}
	
		// texture
		set_texture(NULL);

		// render
		void * buf = NULL;
		if(SUCCEEDED(vbuffer_->Lock(0, sizeof(vertices), &buf, 0)))
		{
			memcpy(buf, vertices, sizeof(vertices));
			vbuffer_->Unlock();

			if(fill)
				device_->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
			else
				device_->DrawPrimitive(D3DPT_LINESTRIP,0,4);
		}
	}

	void D3D9Renderer::draw_line(Point const & pt1, Point const & pt2, Color const & clr1, Color const & clr2)
	{
		CustomVertex vertices[2];

		// vertex 1
		vertices[0].color = clr1.argb();
		vertices[0].dest_pt = pt1;

		// vertex 2
		vertices[1].color = clr2.argb();
		vertices[1].dest_pt = pt2;


		// texture
		set_texture(NULL);

		// render
		void * buf = NULL;
		if(SUCCEEDED(vbuffer_->Lock(0, sizeof(vertices), &buf, 0)))
		{
			memcpy(buf, vertices, sizeof(vertices));
			vbuffer_->Unlock();

			device_->DrawPrimitive(D3DPT_LINELIST, 0, 1);
		}
	}


	void D3D9Renderer::set_texture(LPDIRECT3DTEXTURE9 tex)
	{
		if(cur_texture_ != tex)
		{
			cur_texture_ = tex;
			device_->SetTexture(0, tex);
		}
	}

	uint32 D3D9Renderer::map_blend(Blend blend)
	{
		switch(blend)
		{
		case BL_MODULATE:
			return D3DTOP_MODULATE;

		case BL_MODULATE2X:
			return D3DTOP_MODULATE2X;

		case BL_MODULATE4X:
			return D3DTOP_MODULATE4X;

		case BL_ADD:
			return D3DTOP_ADD;

		default:
			BOOST_ASSERT(false);
			return D3DTOP_MODULATE;
		}
	}
}

#pragma warning(pop)
