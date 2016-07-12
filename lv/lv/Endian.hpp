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

#include <boost/detail/endian.hpp>

#include <algorithm>
#include <type_traits>


namespace lv
{
	template<typename T>
	T	endian_switch(T t, bool s = true)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic or enum type only");

		if (sizeof(t) == 1 || ! s)
		{
			return t;
		}
		else
		{
			T ret;

			uint8 * pold = reinterpret_cast<uint8*>(&t);
			uint8 * pnew = reinterpret_cast<uint8*>(&ret);

			std::reverse_copy(pold, pold + sizeof(t), pnew);

			return ret;
		}
	}

	inline	bool	little_endian()
	{
#ifdef BOOST_LITTLE_ENDIAN
		return true;
#else
		return false;
#endif
	}

	inline	bool	big_endian()
	{
		return ! little_endian();
	}
}

#endif