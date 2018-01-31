// *********************************************************************
//  Exec   version:  1.0   ¡¤  date: 08/22/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/lvlib2.hpp>

#include <cstddef>


struct lua_State;

namespace lv::lua
{
	using pcall_handler = int (*)(lua_State * L);

	inline int default_pcall_handler(lua_State * L)
	{
		return 1;
	}


	/// @exception runtime_error
	/// @name set to str if name is nullptr
	void dostr(lua_State * L, char const * str, std::size_t size, char const * name = nullptr, pcall_handler h = default_pcall_handler);


}

