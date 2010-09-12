// *********************************************************************
//  Point   version:  1.0   ¡¤  date: 09/07/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GRAPHICS_STREAM_POINT_HPP
#define LV_GRAPHICS_STREAM_POINT_HPP

#include <lv/Graphics/Point.hpp>

namespace lv
{
	template<typename T>
	std::ostream & operator << (std::ostream & stream, PointT<T> const & pt)
	{
		return stream << "Point(" << pt.x << ", " << pt.y << ')';
	}
}

#endif