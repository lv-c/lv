// *********************************************************************
//  Pair   version:  1.0   ¡¤  date: 07/20/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <utility>

#include <luabind/class.hpp>
#include <luabind/operator.hpp>


namespace lv::lua
{
	template<class T1, class T2>
	void bind_pair(lua_State * L, char const * name)
	{
		using type = std::pair<T1, T2>;
		using namespace luabind;

		module(L)
		[
			class_<type>(name)
				.def(constructor<>())
				.def(constructor<T1 const &, T2 const &>())
				.def_readwrite("first", &type::first)
				.def_readwrite("second", &type::second)
				.def(self == type())
		];
	}
}