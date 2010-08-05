// *********************************************************************
//  Endian   version:  1.0   ¡¤  date: 08/05/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ENDIAN_HPP
#define LV_ENDIAN_HPP

#include <lv/IntType.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace lv
{
	template<typename T>
	T	endian_switch(T t)
	{
		BOOST_MPL_ASSERT((boost::is_integral<T>));

		T ret = t;
		uint8 * front = reinterpret_cast<uint8*>(&ret);
		uint8 * back = reinterpret_cast<uint8*>(&ret) + sizeof(T) - 1;

		while(front < back)
		{
			std::swap(*front, *back);
			
			front++;
			back--;
		}

		return ret;
	}
}

#endif