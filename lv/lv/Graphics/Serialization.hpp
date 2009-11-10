// *********************************************************************
//  Serialization   version:  1.0   ¡¤  date: 11/11/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GRAPHICS_SERIALIZATION_HPP
#define LV_GRAPHICS_SERIALIZATION_HPP

#include <lv/Graphics/Point.hpp>


namespace boost { namespace serialization {


	template<typename Archive, typename T>
	void serialize(Archive & ar, lv::PointT<T> & pt, unsigned int)
	{
		ar & pt.x & pt.y;
	}

	template<typename Archive, typename T>
	void serialize(Archive & ar, lv::SizeT<T> & sz, unsigned int)
	{
		ar & sz.cx & sz.cy;
	}

	template<typename Archive, typename T>
	void serialize(Archive & ar, lv::RectT<T> & rc, unsigned int)
	{
		ar & rc.left & rc.top & rc.right & rc.bottom;
	}

} }

#endif