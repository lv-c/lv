// *********************************************************************
//  NonintrusiveOptional   version:  1.0   ��  date: 2014/03/09
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_NONINTRUSIVE_OPTIONAL_HPP
#define LV_LUAARCHIVE_NONINTRUSIVE_OPTIONAL_HPP

#include <lv/LuaArchive/Fwd.hpp>
#include <lv/Serialization/NonintrusiveOptional.hpp>

namespace lv::lua::archive
{
	template<class T>
	void	load(luabind::object const & obj, lv::serialization::NonintrusiveOptional<T> & t)
	{
		if (luabind::type(obj) != LUA_TNIL)
		{
			load_adl(obj, t.value());
		}
	}


	template<class T>
	void	load(Parser & parser, lv::serialization::NonintrusiveOptional<T> & t)
	{
		if (!parser.read_if("nil"))
		{
			parser >> t.value();
		}
	}

}

#endif
