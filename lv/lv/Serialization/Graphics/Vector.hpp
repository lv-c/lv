// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 08/31/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_GRAPHICS_VECTOR_HPP
#define LV_SERIALIZATION_GRAPHICS_VECTOR_HPP

#include <lv/Graphics/Vector.hpp>

namespace boost { namespace serialization {

	template<typename Archive, typename T>
	void serialize(Archive & ar, lv::VectorT<T> & vec, unsigned int)
	{
		ar & vec.x & vec.y & vec.z;
	}

} }

#endif