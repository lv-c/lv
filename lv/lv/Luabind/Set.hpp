// *********************************************************************
//  Set   version:  1.0   ¡¤  date: 07/20/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_SET_HPP
#define LV_LUABIND_SET_HPP

#include <lv/Lua/Bind/Pair.hpp>

#include <set>
#include <string>

namespace lv
{
	template<typename T>
	void bind_set(lua_State * state, char const * name)
	{
		typedef std::set<T>	type;
		using namespace luabind;

		module(state)
		[
			class_<type>(name)
				.def(constructor<>())
				.def("size", &type::size)
				.def("empty", &type::empty)
				.def("clear", &type::clear)
				.def("count", &type::count)
				.def("insert", (std::pair<type::iterator, bool>(type::*)(T const &)) &type::insert)
				.def("erase", (type::size_type(type::*)(T const &)) &type::erase)
				.scope
				[
					class_<type::iterator>((std::string(name) + "_it").c_str())
						.def(self == type::iterator())
				]
		];

		// used by type::insert
		bind_pair<type::iterator, bool>(state, (std::string(name) + "_ib_pair").c_str());
	}
}

#endif