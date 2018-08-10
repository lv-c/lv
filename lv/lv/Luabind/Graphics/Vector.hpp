// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 09/10/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Vector.hpp>
#include <lv/Graphics/Stream/Vector.hpp>

#include <luabind/class.hpp>
#include <luabind/operator.hpp>


namespace lv::lua::graphics
{
	template<class T>
	void	bind_vector(lua_State * L, char const * name)
	{
		using type = VectorT<T>;
		using namespace luabind;

		module(L)
		[
			class_<type>(name)
				.def(constructor<>())
				.def(constructor<T, T, T>())
				.def(constructor<type const &>())
				.def_readwrite("x", &type::x)
				.def_readwrite("y", &type::y)
				.def_readwrite("z", &type::z)
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