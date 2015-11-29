// *********************************************************************
//  Extract   version:  1.0   ¡¤  date: 2012/09/27
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUA_EXTRACT_HPP
#define LV_LUA_EXTRACT_HPP

#include <lv/LuaArchive/LuaIArchive.hpp>

#include <luabind/object.hpp>

namespace lv { namespace lua {

	template<class T>
	T	extract(luabind::object const & obj)
	{
		if(luabind::type(obj) != LUA_TTABLE)
		{
			return luabind::object_cast<T>(obj);
		}
		else
		{
			T t;
			archive::load(obj, t);

			return t;
		}
	}

} }

#endif