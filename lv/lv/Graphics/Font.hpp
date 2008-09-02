// *********************************************************************
//  Font   version:  1.0   ¡¤  date: 08/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FONT_HPP
#define LV_FONT_HPP

#include <lv/Graphics/ColorRect.hpp>

namespace lv
{
	
	class Interpreter
	{
	public:



		void	operator () (std::wstring const & txt, bool calc_rect)
		{

		}
	};

	class Formater
	{

	};


	class Font
	{
	public:

		enum TextFormat
		{

		};

		void	draw_text(std::wstring const & txt, Rect const & rect, ColorRect const & colors, TextFormat format);

	};
}

#endif // LV_FONT_HPP