// *********************************************************************
//  TableBuilder   version:  1.0   ¡¤  date: 2012/11/11
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Lua/TableBuilder.hpp>
#include <lv/LuaArchive/Vector.hpp>
#include <lv/LuaArchive/Map.hpp>


BOOST_AUTO_TEST_CASE(test_table_builder)
{
	std::vector<int> vec;
	vec.push_back(4);
	vec.push_back(6);

	std::ostringstream oss;

	oss << LV_TBL("name", "Jim")("age", 16)("arr", vec)("sub", LV_TBL("sub_name", 1))("arr2", LV_ARR("a", "b", "c"));

	BOOST_CHECK_EQUAL(oss.str(), "{name = 'Jim', age = 16, arr = {4, 6}, sub = {sub_name = 1}, arr2 = {'a', 'b', 'c'}}");

	//

	std::map<int, std::string> m;
	m[1] = "you";
	m[3] = "me";

	oss.str(std::string());
	oss << "map: " << LV_OBJ(m);

	BOOST_CHECK_EQUAL(oss.str(), "map: {\n\t[1] = 'you', \n\t[3] = 'me'\n}");
}