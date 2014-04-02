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

#include <boost/optional.hpp>

namespace lv { namespace lua { namespace archive {


	template<typename T>
	void	save(std::ostream & os, boost::optional<T> const & t, size_t level)
	{
		if(t)
		{
			save(os, *t, level);
		}
		else
		{
			os << "nil";
		}
	}


	template<typename T>
	void	load(luabind::object const & obj, boost::optional<T> & t)
	{
		if(luabind::type(obj) == LUA_TNIL)
		{
			t.reset();
		}
		else
		{
			if(! t)
			{
				t = T();
			}
			load(obj, *t);
		}
	}

	template<typename T>
	void	load(Parser & parser, boost::optional<T> & t)
	{
		if(parser.read_if("nil"))
		{
			t.reset();
		}
		else
		{
			if(! t)
			{
				t = T();
			}
			parser >> *t;
		}
	}

} } }

#endif