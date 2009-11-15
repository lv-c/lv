// *********************************************************************
//  Graphics   version:  1.0   ¡¤  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUA_BIND_GRAPHICS_HPP
#define LV_LUA_BIND_GRAPHICS_HPP

#include <lv/Graphics/Rect.hpp>
#include <lv/Graphics/Streaming.hpp>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

namespace lv { namespace lua {
	
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
				.def(tostring(self))
				.def(self + type())
				.def(self - type())
				.def(self * T())
				.def(self / T())
				.def(self == type())
		];
	}

	template<typename T>
	void	bind_size(lua_State* state, char const * name)
	{
		typedef SizeT<T> type;
		using namespace luabind;

		module(state)
		[
			class_<type>(name)
				.def(constructor<>())
				.def(constructor<T, T>())
				.def_readwrite("cx", &type::cx)
				.def_readwrite("cy", &type::cy)
				.def("set", &type::set)
				.def(tostring(self))
				.def(self + type())
				.def(self - type())
				.def(self * T())
				.def(self / T())
				.def(self == type())
		];
	}
	

	template<typename T>
	void	bind_rect(lua_State* state, char const * name)
	{
		typedef RectT<T> type;
		typedef PointT<T> point_type;
		typedef SizeT<T> size_type;

		using namespace luabind;

		module(state)
		[
			class_<type>(name)
				.def(constructor<>())
				.def(constructor<T, T, T, T>())
				.def(constructor<point_type const&, point_type const&>())
				.def(constructor<point_type const&, size_type const&>())
				.def_readwrite("left", &type::left)
				.def_readwrite("top", &type::top)
				.def_readwrite("right", &type::right)
				.def_readwrite("bottom", &type::bottom)
				.def("set", &type::set)
				.def("move_to", &type::move_to)
				.def("resize", &type::resize)
				.def("pos", &type::pos)
				.def("size", &type::size)
				.def("height", &type::height)
				.def("width", &type::width)
				.def("contains", (bool(type::*)(point_type const&))&type::contains)
				.def("contains", (bool(type::*)(type const&))&type::contains)
				.def(tostring(self))
				.def(self + point_type())
				.def(self - point_type())
				.def(self * T())
				.def(self / T())
				.def(self == type())
		];
	}

} }

#endif 
