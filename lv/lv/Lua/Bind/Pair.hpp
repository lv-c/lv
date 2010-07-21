// *********************************************************************
//  Pair   version:  1.0   ¡¤  date: 07/20/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUA_BIND_PAIR_HPP
#define LV_LUA_BIND_PAIR_HPP

#include <utility>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

namespace lv
{
	template<typename T1, typename T2>
	void bind_pair(lua_State * state, char const * name)
	{
		typedef std::pair<T1, T2>	type;
		using namespace luabind;

		module(state)
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

#endif