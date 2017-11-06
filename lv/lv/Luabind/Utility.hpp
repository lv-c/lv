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

#include <string>
#include <type_traits>


namespace lv::lua
{
	template<class T>
	struct is_primitive
		: std::integral_constant<bool, 
			std::is_arithmetic<T>::value || std::is_same<T, std::string>::value>
	{
	};

}

#endif