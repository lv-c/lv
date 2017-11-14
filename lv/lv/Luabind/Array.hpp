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

#include <lv/Luabind/Utility.hpp>

#include <luabind/class.hpp>

#include <array>


namespace lv::lua
{
	template<class T, size_t N>
	void bind_array(lua_State * L, char const* name)
	{
		typedef std::array<T, N>	type;
		typedef typename type::const_reference	const_reference;
		typedef typename type::reference		reference;
		typedef typename type::size_type		size_type;

		using namespace luabind;

		class_<type> arr(name);

		arr
			.def("assign", &type::assign)
			.def("size", &type::size)
			.def("at", (const_reference(type::*)(size_type) const)&type::at)
			.def("front", (const_reference(type::*)() const)&type::front)
			.def("back", (const_reference(type::*)() const)&type::back)
		;

		if (!is_primitive<T>::value)
		{
			arr
				.def("at", (reference(type::*)(size_type))&type::at)
				.def("front", (reference(type::*)())&type::front)
				.def("back", (reference(type::*)())&type::back)
			;
		}

		module(L)
		[
			arr
		];
	}
}


#endif 