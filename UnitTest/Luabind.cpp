// *********************************************************************
//  Luabind   version:  1.0   ¡¤  date: 2012/04/18
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Luabind/Vector.hpp>
#include <lv/Lua/Exec.hpp>

#include <lua.hpp>

BOOST_AUTO_TEST_CASE(test_luabind)
{
	lua_State * state = lua_open();

	luaopen_base(state);
	luabind::open(state);

	lv::lua::bind_vector<int>(state, "IntVec");

	char const * script = 
		"local v = IntVec()\n"
		"v:push_back(1)\n"
		"local i = v:at(0)\n"
		;

	lv::lua::dostr(state, script);

	lua_close(state);
}