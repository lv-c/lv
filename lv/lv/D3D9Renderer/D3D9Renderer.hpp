// *********************************************************************
//  D3D9Renderer   version:  1.0   ¡¤  date: 08/15/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_D3D9RENDERER_HPP
#define LV_D3D9RENDERER_HPP

#include <lv/Graphics/Renderer.hpp>
#include <lv/Graphics/Point.hpp>
#include <d3d9.h>

namespace lv
{
	class D3D9Renderer : public Renderer
	{
		LPDIRECT3DDEVICE9	device_;

		LPDIRECT3DVERTEXBUFFER9	vbuffer_;


		struct CustomVertex
		{
			CustomVertex()
				: sz(0.0f)
				, rhw(1.0f)
			{
			}

			PointT<float>	dest_pt;
			float sz, rhw;
			uint32 color;       
			PointT<float>	tex_pt;
		};

		static uint32 const	VERTEX_FVF;

		Blend	cur_blend_;
		LPDIRECT3DTEXTURE9	cur_texture_;

	private:

		uint32	map_blend(Blend blend);

		// If tex is not the same as the current texture, set the current texture to tex.
		void	set_texture(LPDIRECT3DTEXTURE9 tex);

	public:

		D3D9Renderer(LPDIRECT3DDEVICE9 device);


		LPDIRECT3DDEVICE9	device() const
		{
			return this->device_;
		}


		/**
		 * @exception std::runtime_error on failure
		 */
		virtual	void	begin_scene();
		
		virtual	void	end_scene();

		virtual	void	draw_quad(Point const & dest_pos, Texture const & tex, Rect const & src_rect,
			ColorRect const & colors = ColorRect::white, Blend blend = BL_MODULATE, Size const & dest_size = Size());


		virtual	void	frame_quad(Rect const & rect, bool fill, ColorRect const & colors);

		virtual	void	draw_line(Point const & pt1, Point const & pt2, Color const & clr1, Color const & clr2);
	};
}

#endif // LV_D3D9RENDERER_HPP