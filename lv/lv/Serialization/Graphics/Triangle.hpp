// *********************************************************************
//  Triangle   version:  1.0   ¡¤  date: 09/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_TRIANGLE_HPP
#define LV_SERIALIZATION_TRIANGLE_HPP

#include <lv/Graphics/Triangle.hpp>

namespace boost { namespace serialization {

	template<typename Archive, typename T>
	void serialize(Archive & ar, lv::TriangleT<T> & tri, unsigned int)
	{
		ar & tri.v0 & tri.v1 & tri.v2;
	}

} }

#endif
