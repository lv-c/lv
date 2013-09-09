// *********************************************************************
//  Exec   version:  1.0   ¡¤  date: 08/22/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUA_EXEC_HPP
#define LV_LUA_EXEC_HPP

#include <lv/lvlib2.hpp>

struct lua_State;

namespace lv { namespace lua {

	typedef int (* pcall_handler)(lua_State * L);

	inline int default_pcall_handler(lua_State * L)
	{
		return 1;
	}


	/// @exception runtime_error
	/// @name set to str if name is NULL
	void dostr(lua_State * L, char const * str, size_t size, char const * name = NULL, pcall_handler h = default_pcall_handler);


} }


#endif
