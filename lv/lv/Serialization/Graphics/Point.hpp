// *********************************************************************
//  Point   version:  1.0   ¡¤  date: 08/31/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_GRAPHICS_POINT_HPP
#define LV_SERIALIZATION_GRAPHICS_POINT_HPP


#include <lv/Graphics/Point.hpp>

namespace boost { namespace serialization {

	template<typename Archive, typename T>
	void serialize(Archive & ar, lv::PointT<T> & pt, unsigned int)
	{
		ar & pt.x & pt.y;
	}

} }

#endif