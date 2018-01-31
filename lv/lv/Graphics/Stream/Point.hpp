// *********************************************************************
//  Point   version:  1.0   ¡¤  date: 09/07/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Point.hpp>

namespace lv
{
	template<class T>
	std::ostream & operator << (std::ostream & stream, PointT<T> const & pt)
	{
		return stream << "Point(" << pt.x << ", " << pt.y << ')';
	}
}