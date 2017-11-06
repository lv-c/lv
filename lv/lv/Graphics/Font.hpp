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

#include <lv/Config.hpp>
#include <lv/Graphics/ColorRect.hpp>
#include <lv/Graphics/Rect.hpp>
#include <lv/Graphics/RenderFwd.hpp>

#include <string>


namespace lv
{
	
	enum TextFormat
	{
		// horizontal alignment
		TF_LEFT		=	1 << 0, 
		TF_CENTER	=	1 << 1,
		TF_RIGHT	=	1 << 2,

		// vertical alignment
		TF_TOP		=	1 << 3,
		TF_V_CENTER	=	1 << 4,
		TF_BOTTOM	=	1 << 5

		// text will not be broken into multiple lines to fit the rectangle
		// TF_NO_CLIP
	};



	class Font
	{
			
	protected:
		std::string		name_;		// identifier of this font.

		size_t			height_;	// height of the font

	public:

		Font(std::string const & name, size_t height);

		/**
		 * Retrieve the identifier of this font.
		 */
		std::string	const &	name() const
		{
			return this->name_;
		}

		/**
		 * Retrieve the height of the font.
		 */
		size_t	height() const
		{
			return this->height_;
		}

		/** 
		 * check if the font has been loaded.
		 */
		virtual bool	loaded() const
		{
			return false;
		}

		Size	calc_size(std::wstring const & txt, int max_width = -1);
		
		void	draw_text(std::wstring const & txt, Rect const & rect, ColorRect const & colors, int format = TF_TOP | TF_LEFT);

		// low-level operations which allow users to implement complex text renderers
		virtual	void	draw_char(wchar_t c, Point const & pt, ColorRect const & colors) = 0;

		virtual	int		advance(wchar_t c) = 0;

	protected:

		virtual	void	update_cache(std::wstring const & txt) {}

	};
}

#endif