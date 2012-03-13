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
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_enum.hpp>

#include <algorithm>

namespace lv
{
	template<typename T>
	T	endian_switch(T t)
	{
		BOOST_MPL_ASSERT((boost::mpl::or_<boost::is_arithmetic<T>, boost::is_enum<T> >));

		if(sizeof(t) == 1)
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
}

#endif