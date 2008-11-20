// *********************************************************************
//  Renderer   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RENDERER_HPP
#define LV_RENDERER_HPP


#include <lv/Singleton.hpp>
#include <lv/Graphics/ColorRect.hpp>
#include <lv/Graphics/Rect.hpp>

namespace lv
{
	class Texture;

	class Renderer : public Singleton<Renderer>
	{
	public:

		enum Blend
		{
			BL_MODULATE,
			BL_MODULATE2X,
			BL_MODULATE4X,
			BL_ADD
		};

		virtual	Size	max_texture_size() = 0;

		virtual	void	begin_scene() = 0;
		virtual	void	end_scene() = 0;

		virtual	void	clear(Color const & color = Color()) = 0;
		virtual	void	present() = 0;


		virtual	void	clip(Rect const & rect) = 0;

		/**
		 * Draws a quad.
		 * @param tex The texture used to render
		 * @param dest_size Size of the destination quad. If it's Size(), the size of the destination quad
		 *		will be the same as that of the texture quad.
		 */
		virtual	void	draw_quad(Point const & dest_pos, Texture const & tex, Rect const & src_rect,
			ColorRect const & colors = ColorRect::white, Blend blend = BL_MODULATE, Size const & dest_size = Size()) = 0;


		virtual	void	draw_line(Point const & pt1, Point const & pt2, Color const & clr1, Color const & clr2) = 0;

		virtual	void	frame_quad(Rect const & rect, bool fill, ColorRect const & colors) = 0;
	};
}


#endif // LV_RENDERER_HPP