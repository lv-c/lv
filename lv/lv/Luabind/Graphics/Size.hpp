// *********************************************************************
//  Size   version:  1.0   ¡¤  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_GRAPHICS_SIZE_HPP
#define LV_LUABIND_GRAPHICS_SIZE_HPP

#include <lv/Graphics/Size.hpp>
#include <lv/Graphics/Stream/Size.hpp>

#include <luabind/class.hpp>
#include <luabind/operator.hpp>

namespace lv { namespace lua { namespace graphics {

	template<typename T>
	void	bind_size(lua_State * L, char const * name)
	{
		typedef SizeT<T> type;
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

} } }

#endif