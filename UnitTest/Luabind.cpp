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
#include <lv/Lua/Extract.hpp>
#include <lv/LuaArchive/Vector.hpp>
#include <lv/Serialization/Graphics/Point.hpp>
#include <lv/Graphics/Stream/Point.hpp>

#include <lua.hpp>

BOOST_AUTO_TEST_CASE(test_luabind)
{
	lua_State * state = lua_open();

	luaopen_base(state);
	luabind::open(state);

	lv::lua::bind_vector<int>(state, "IntVec");

	// all global values
	char const * script = 
		"v = IntVec()\n"
		"v:push_back(1)\n"
		"i = v:at(0)\n"
		"pt = { 10, 20 }\n"
		;

	lv::lua::dostr(state, script, strlen(script));

	luabind::object global = luabind::globals(state);

	std::vector<int> vec = lv::lua::extract<std::vector<int> >(global["v"]);

	BOOST_CHECK_EQUAL(vec.size(), 1);
	BOOST_CHECK_EQUAL(vec[0], 1);

	BOOST_CHECK_EQUAL(lv::lua::extract<int>(global["i"]), 1);

	BOOST_CHECK_EQUAL(lv::lua::extract<lv::Point>(global["pt"]), lv::Point(10, 20));

	// release
	global = luabind::object();

	lua_close(state);
}