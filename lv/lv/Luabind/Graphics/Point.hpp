// *********************************************************************
//  Point   version:  1.0   ¡¤  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_GRAPHICS_POINT_HPP
#define LV_LUABIND_GRAPHICS_POINT_HPP

#include <lv/Graphics/Point.hpp>
#include <lv/Graphics/Stream/Point.hpp>

#include <luabind/class.hpp>
#include <luabind/operator.hpp>

namespace lv { namespace lua { namespace graphics {

	template<typename T>
	void	bind_point(lua_State* state, char const * name)
	{
		typedef PointT<T> type;
		using namespace luabind;

		module(state)
		[
			class_<type>(name)
				.def(constructor<>())
				.def(constructor<T, T>())
				.def(constructor<type const&>())
				.def_readwrite("x", &type::x)
				.def_readwrite("y", &type::y)
				.def("set", &type::set)
				.def(tostring(const_self))
				.def(self + type())
				.def(self - type())
				.def(self * T())
				.def(self / T())
				.def(self == type())
		];
	}

} } }

#endif