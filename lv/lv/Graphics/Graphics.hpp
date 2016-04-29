// *********************************************************************
//  Graphics   version:  1.0   ¡¤  date: 2013/03/15
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GRAPHICS_GRAPHICS_HPP
#define LV_GRAPHICS_GRAPHICS_HPP

namespace lv { namespace graphics {

	template<class Action>
	void	line(int sx, int sy, int ex, int ey, Action act)
	{
		int dx = std::abs(ex - sx);
		int dy = std::abs(ey - sy);

		bool reversed = false;

		if (dx < dy)
		{
			reversed = true;

			std::swap(sx, sy);
			std::swap(ex, ey);
			std::swap(dx, dy);
		}

		int tx = (ex - sx) > 0 ? 1 : -1;
		int ty = (ey - sy) > 0 ? 1 : -1;

		int curx = sx;
		int cury = sy;
		int inc1 = 2 * dy;
		int inc2 = 2 * (dy - dx);
		int d = inc1 - dx;

		while (curx != ex)
		{
			if (d < 0)
			{
				d += inc1;
			}
			else
			{
				cury += ty;
				d += inc2;
			}

			if (reversed)
			{
				act(cury, curx);
			}
			else
			{
				act(curx, cury);
			}

			curx += tx;
		}
	}

	template<class Img, class Clr>
	class Shader
	{
		Img &	img_;		// gil image
		Clr		clr_;

	public:

		Shader(Img & img, Clr clr)
			: img_(img)
			, clr_(clr)
		{
		}

		void operator () (int x, int y)
		{
			img_(x, y) = clr_;
		}
	};

	template<class Img, class Clr>
	Shader<Img, Clr>	make_shader(Img & img, Clr clr)
	{
		return Shader<Img, Clr>(img, clr);
	}

} }

#endif