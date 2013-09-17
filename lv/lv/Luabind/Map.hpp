// *********************************************************************
//  Map   version:  1.0   ¡¤  date: 2012/11/05
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_MAP_HPP
#define LV_LUABIND_MAP_HPP

#include <lv/Luabind/Pair.hpp>

#include <map>

namespace lv { namespace lua {

	template<typename K, typename V>
	void bind_map(lua_State * L, char const * name)
	{
		typedef std::map<K, V>	type;
		typedef typename type::key_type		key_type;
		typedef typename type::value_type	value_type;
		typedef typename type::mapped_type	mapped_type;
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
				.def("count", &type::count)
				.def("begin", (iterator(type::*)()) &type::begin)
				.def("_end", (iterator(type::*)()) &type::end)
				.def("insert", (std::pair<iterator, bool>(type::*)(value_type const &)) &type::insert)
				.def("erase", (size_type(type::*)(key_type const &)) &type::erase)
				.def("find", (iterator(type::*)(key_type const &)) &type::find)
				.scope
				[
					class_<type::iterator>((std::string(name) + "_it").c_str())
						.def(self == iterator())
					,
					class_<value_type>("pair")
						.def(constructor<key_type const &, mapped_type const &>())
				]
		];

		// used by type::insert
		bind_pair<iterator, bool>(L, (std::string(name) + "_ib_pair").c_str());
	}

} }

#endif