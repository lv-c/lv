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

#include <lv/Luabind/Set.hpp>
#include <lv/Luabind/Map.hpp>
#include <lv/Lua/Exec.hpp>

#include <lua.hpp>

#include <iostream>

#define LV_LIB_NAME "luabind"
#include <lv/AutoLink.hpp>

#define LV_LIB_NAME "lua"
#include <lv/AutoLink.hpp>


int error_handler(lua_State* state)
{
	std::cerr << lua_tostring(state, -1);
	return 1;
}


BOOST_AUTO_TEST_CASE(test_luaset)
{
	lua_State * state = luaL_newstate();

	luaL_openlibs(state);
	luabind::open(state);
	luabind::set_pcall_callback(error_handler);

	lv::lua::bind_set<int>(state, "IntSet");
	lv::lua::bind_map<std::string, int>(state, "StrIntMap");


	char const * str =
		"s = IntSet()\n"
		"assert(s:empty())\n"
		"assert(s:count(10) == 0)\n"
		"assert(s:find(10) == s:_end())\n"
		"assert(s:insert(10).second)\n"
		"assert(not s:insert(10).second)\n"
		"assert(s:count(10) == 1)\n"
		"assert(s:find(10) ~= s:_end())\n"

		"m = StrIntMap()\n"
		"assert(m:empty())\n"
		"m:insert(StrIntMap.pair('a', 10))\n"
		"assert(m:size() == 1)\n"
		"assert(m:find('a') ~= m:_end())\n"
		"assert(m:count('a') == 1)\n"

		"print('luaset test done!')\n"
	;

	lv::lua::dostr(state, str, strlen(str));


	lua_close(state);
}