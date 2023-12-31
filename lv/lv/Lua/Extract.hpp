// *********************************************************************
//  Extract   version:  1.0   ��  date: 2012/09/27
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/LuaArchive/LuaIArchive.hpp>

#include <luabind/object.hpp>


namespace lv::lua
{
	template<class T>
	T	extract(luabind::object const & obj)
	{
		if (luabind::type(obj) != LUA_TTABLE)
		{
			return luabind::object_cast<T>(obj);
		}
		else
		{
			T t;
			archive::load_adl(obj, t);

			return t;
		}
	}

}
