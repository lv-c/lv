// *********************************************************************
//  Optional   version:  1.0   ¡¤  date: 01/21/2011
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2011 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_OPTIONAL_HPP
#define LV_LUAARCHIVE_OPTIONAL_HPP

#include <lv/LuaArchive/Fwd.hpp>

#include <optional>


namespace lv::lua::archive
{
	template<class T>
	void	save(std::ostream & os, std::optional<T> const & t, size_t level)
	{
		if (t)
		{
			save_adl(os, *t, level);
		}
		else
		{
			os << "nil";
		}
	}


	template<class T>
	void	load(luabind::object const & obj, std::optional<T> & t)
	{
		if (luabind::type(obj) == LUA_TNIL)
		{
			t.reset();
		}
		else
		{
			if (!t)
			{
				t = T();
			}
			load_adl(obj, *t);
		}
	}

	template<class T>
	void	load(Parser & parser, std::optional<T> & t)
	{
		if (parser.read_if("nil"))
		{
			t.reset();
		}
		else
		{
			if (!t)
			{
				t = T();
			}
			parser >> *t;
		}
	}

}

#endif
