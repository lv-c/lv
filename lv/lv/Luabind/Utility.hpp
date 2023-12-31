// *********************************************************************
//  Utility   version:  1.0   ��  date: 2012/04/18
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <string>
#include <type_traits>


namespace lv::lua
{
	template<class T>
	struct is_primitive
		: std::integral_constant<bool, 
			std::is_arithmetic_v<T> || std::is_same_v<T, std::string> >
	{
	};


	template<class T>
	constexpr bool	is_primitive_v = is_primitive<T>::value;
}