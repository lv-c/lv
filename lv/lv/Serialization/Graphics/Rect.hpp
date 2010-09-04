// *********************************************************************
//  Rect   version:  1.0   ��  date: 08/31/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_GRAPHICS_RECT_HPP
#define LV_SERIALIZATION_GRAPHICS_RECT_HPP


#include <lv/Graphics/Rect.hpp>

namespace boost { namespace serialization {

	template<typename Archive, typename T>
	void serialize(Archive & ar, lv::RectT<T> & rc, unsigned int)
	{
		ar & rc.left & rc.top & rc.right & rc.bottom;
	}

} }

#endif