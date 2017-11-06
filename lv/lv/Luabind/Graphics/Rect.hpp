// *********************************************************************
//  Rect   version:  1.0   ¡¤  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_GRAPHICS_RECT_HPP
#define LV_LUABIND_GRAPHICS_RECT_HPP

#include <lv/Graphics/Rect.hpp>
#include <lv/Graphics/Stream/Rect.hpp>

#include <luabind/class.hpp>
#include <luabind/operator.hpp>


namespace lv::lua::graphics
{
	template<typename T>
	void	bind_rect(lua_State * L, char const * name)
	{
		typedef RectT<T> type;
		typedef PointT<T> point_type;
		typedef SizeT<T> size_type;

		using namespace luabind;

		module(L)
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
				.def(tostring(const_self))
				.def(self + point_type())
				.def(self - point_type())
				.def(self * T())
				.def(self / T())
				.def(self == type())
		];
	}

}

#endif