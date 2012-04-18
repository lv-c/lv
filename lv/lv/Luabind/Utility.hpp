// *********************************************************************
//  Utility   version:  1.0   ¡¤  date: 2012/04/18
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_UTILITY_HPP
#define LV_LUABIND_UTILITY_HPP

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/mpl/or.hpp>

#include <string>

namespace lv { namespace lua {

	template<class T>
	struct is_primitive
		: boost::mpl::or_<
			boost::is_arithmetic<T>,
			boost::is_same<T, std::string>
		>
	{
	};

} }

#endif