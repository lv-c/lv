// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 09/10/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Vector.hpp>

namespace lv
{
	template<class T>
	std::ostream & operator << (std::ostream & stream, VectorT<T> const & vec)
	{
		return stream << "Vector(" << vec.x << ", " << vec.y << ", " << vec.z << ')';
	}
}