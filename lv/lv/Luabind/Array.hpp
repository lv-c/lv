// *********************************************************************
//  Array   version:  1.0   ¡¤  date: 03/14/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_ARRAY_HPP
#define LV_LUABIND_ARRAY_HPP

#include <boost/array.hpp>

#include <luabind/class.hpp>

namespace lv { namespace lua {

	template<class T, size_t N>
	void bind_array(lua_State* state, char const* name)
	{
		typedef boost::array<T, N>	type;
		using namespace luabind;

		module(state)
		[
			class_<type>(name)
				.def("assign", &type::assign)
				.def("size", &type::size)
				.def("at", (type::reference(type::*)(type::size_type))&type::at)
				.def("at", (type::const_reference(type::*)(type::size_type) const)&type::at)
				.def("front", (type::reference(type::*)())&type::front)
				.def("front", (type::const_reference(type::*)() const)&type::front)
				.def("back", (type::reference(type::*)())&type::back)
				.def("back", (type::const_reference(type::*)() const)&type::back)
		];
	}
} }


#endif 