// *********************************************************************
//  Size   version:  1.0   ��  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Size.hpp>
#include <lv/Graphics/Stream/Size.hpp>

#include <luabind/class.hpp>
#include <luabind/operator.hpp>


namespace lv::lua::graphics
{
	template<class T>
	void	bind_size(lua_State * L, char const * name)
	{
		using type = SizeT<T>;
		using namespace luabind;

		module(L)
		[
			class_<type>(name)
				.def(constructor<>())
				.def(constructor<T, T>())
				.def_readwrite("cx", &type::cx)
				.def_readwrite("cy", &type::cy)
				.def("set", &type::set)
				.def(tostring(const_self))
				.def(self + type())
				.def(self - type())
				.def(self * T())
				.def(self / T())
				.def(self == type())
		];
	}

}