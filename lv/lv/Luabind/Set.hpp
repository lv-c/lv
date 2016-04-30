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

#include <lv/Luabind/Pair.hpp>

#include <set>
#include <string>

namespace lv { namespace lua {

	template<typename T>
	void bind_set(lua_State * L, char const * name)
	{
		typedef std::set<T>	type;
		typedef typename type::key_type		key_type;
		typedef typename type::value_type	value_type;
		typedef typename type::iterator		iterator;
		typedef typename type::size_type	size_type;

		using namespace luabind;

		module(L)
		[
			class_<type>(name)
				.def(constructor<>())
				.def("size", &type::size)
				.def("empty", &type::empty)
				.def("clear", &type::clear)
				.def("count", (size_type(type::*)(key_type const &) const) &type::count)
				.def("begin", (iterator(type::*)()) &type::begin)
				.def("_end", (iterator(type::*)()) &type::end)
				.def("insert", (std::pair<iterator, bool>(type::*)(value_type const &)) &type::insert)
				.def("erase", (size_type(type::*)(key_type const &)) &type::erase)
				.def("find", (iterator(type::*)(key_type const &)) &type::find)
				.scope
				[
					class_<iterator>((std::string(name) + "_it").c_str())
						.def(self == iterator())
				]
		];

		// used by type::insert
		bind_pair<iterator, bool>(L, (std::string(name) + "_ib_pair").c_str());
	}
} }

#endif