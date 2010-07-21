// *********************************************************************
//  LuaSet   version:  1.0   ¡¤  date: 07/21/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Lua/Bind/Set.hpp>

#include <lua.hpp>

#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "libluabindd.lib")
#else
#pragma comment(lib, "libluabind.lib")
#endif
#pragma comment(lib, "lua.lib")


int pcall_handler(lua_State* L)
{
	return 1;
}

int error_handler(lua_State* state)
{
	std::cerr << lua_tostring(state, -1);
	return 1;
}

void dostring(lua_State* state, char const* str)
{
	lua_pushcclosure(state, &pcall_handler, 0);

	if (luaL_loadbuffer(state, str, std::strlen(str), str))
	{
		std::string err(lua_tostring(state, -1));
		lua_pop(state, 2);
		throw err;
	}

	if (lua_pcall(state, 0, 0, -2))
	{
		std::string err(lua_tostring(state, -1));
		lua_pop(state, 2);
		throw err;
	}

	lua_pop(state, 1);
}


BOOST_AUTO_TEST_CASE(test_luaset)
{
	lua_State * state = lua_open();

	luaopen_base(state);
	luabind::open(state);
	luabind::set_pcall_callback(error_handler);

	lv::bind_set<int>(state, "IntSet");

	dostring(state, 
		"s = IntSet()\n"
		"assert(s:empty())\n"
		"assert(s:count(10) == 0)\n"
		"assert(s:insert(10).second)\n"
		"assert(not s:insert(10).second)\n"
		"assert(s:count(10) == 1)\n"
		"print('luaset test done!')\n"
	);

	lua_close(state);
}