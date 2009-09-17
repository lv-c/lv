// *********************************************************************
//  Streaming   version:  1.0   ¡¤  date: 09/07/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GRAPHICS_STREAMING_HPP
#define LV_GRAPHICS_STREAMING_HPP

#include <lv/Graphics/Rect.hpp>

namespace lv
{
	inline std::ostream & operator << (std::ostream & stream, Point const & pt)
	{
		return stream << "Point(" << pt.x << ", " << pt.y << ')';
	}

	inline std::ostream & operator << (std::ostream & stream, Size const & sz)
	{
		return stream << "Size(" << sz.cx << ", " << sz.cy << ')';
	}

	inline std::ostream & operator << (std::ostream & stream, Rect const & rc)
	{
		return stream << "Rect(" << rc.left << ", " << rc.top << ", " << rc.right
			<< ", " << rc.bottom << ')';
	}
}

#endif // LV_GRAPHICS_STREAMING_HPP