// *********************************************************************
//  Size   version:  1.0   ��  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_GRAPHICS_SIZE_HPP
#define LV_SERIALIZATION_GRAPHICS_SIZE_HPP

#include <lv/Graphics/Size.hpp>


namespace boost::serialization
{
	template<typename Archive, typename T>
	void serialize(Archive & ar, lv::SizeT<T> & sz, unsigned int)
	{
		ar & sz.cx & sz.cy;
	}

}

#endif