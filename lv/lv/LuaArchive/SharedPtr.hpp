// *********************************************************************
//  SharedPtr   version:  1.0   ¡¤  date: 10/27/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_SHAREDPTR_HPP
#define LV_LUAARCHIVE_SHAREDPTR_HPP

#include <luabind/object.hpp>

#include <boost/shared_ptr.hpp>

namespace lv { namespace lua { namespace archive {

	template<typename T>
	void	save(std::ostream & os, boost::shared_ptr<T> const & v, size_t level)
	{
		if(v)
		{
			save(os, *v, level);
		}
		else
		{
			os << "nil";
		}
	}

	template<typename T>
	void	load(luabind::object const & obj, boost::shared_ptr<T> & v)
	{
		if(luabind::type(obj) == LUA_TNIL)
		{
			v.reset();
		}
		else
		{
			if(! v)
			{
				v.reset(new T());
			}
			load(obj, *v);
		}
	}

	template<typename T>
	void	load(Parser & parser, boost::shared_ptr<T> & v)
	{
		if(parser.read_if("nil"))
		{
			v.reset();
		}
		else
		{
			if(! v)
			{
				v.reset(new T());
			}
			parser >> *v;
		}
	}

} } }

#endif
