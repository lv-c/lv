// *********************************************************************
//  LuaIArchive   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_LUAIARCHIVE_HPP
#define LV_LUAARCHIVE_LUAIARCHIVE_HPP

#include <lv/LuaArchive/Fwd.hpp>
#include <lv/LuaArchive/ISerializer.hpp>

#include <boost/noncopyable.hpp>

namespace lv
{
	class LuaIArchive : boost::noncopyable
	{
		luabind::object	table_;

	public:

		typedef boost::mpl::true_	is_loading;
		typedef boost::mpl::false_	is_saving;

		explicit LuaIArchive(luabind::object const & table)
			: table_(table)
		{
		}
		
		template<typename T>
		LuaIArchive const & operator >> (T & t) const
		{
			lua::archive::load(table_, t);
			return *this;
		}

		template<typename T>
		LuaIArchive const & operator & (T & t) const
		{
			return *this >> t;
		}
	};
}


#endif