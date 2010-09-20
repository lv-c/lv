// *********************************************************************
//  LuaArchive   version:  1.0   ·  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/IntType.hpp>
#include <lv/LuaArchive/LuaIArchive.hpp>
#include <lv/LuaArchive/LuaOArchive.hpp>
#include <lv/LuaArchive/Vector.hpp>
#include <lv/LuaArchive/Map.hpp>
#include <lv/Lua/Exec.hpp>

#include <boost/assign.hpp>

#include <luabind/luabind.hpp>

#include <iostream>
#include <sstream>

#include <lua.hpp>

using namespace std;
using namespace lv;


struct Point
{
	int		x;
	int		y;

	Point()
		: x(0)
		, y(0)
	{
	}

	Point(int x, int y)
		: x(x)
		, y(y)
	{
	}

	template<class Archive>
	void	serialize(Archive & ar, unsigned int version)
	{
		BOOST_CHECK_EQUAL(version, 0);

		ar & x & y;
	}

	bool operator == (Point const & rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator < (Point const & rhs) const
	{
		if(y == rhs.y)
			return x < rhs.x;
		else
			return y < rhs.y;
	}
};

struct Color
{
	uint8	r;
	uint8	g;
	uint8	b;

	Color()
		: r(0)
		, g(0)
		, b(0)
	{
	}

	Color(uint8 r, uint8 g, uint8 b)
		: r(r)
		, g(g)
		, b(b)
	{
	}

	bool operator == (Color const & rhs) const
	{
		return r == rhs.r && g == rhs.g && b == rhs.b;
	}

	template<class Archive>
	void	serialize(Archive & ar, unsigned int version)
	{
		BOOST_CHECK_EQUAL(version, 1);

		ar & boost::serialization::make_nvp("r", r) &
			boost::serialization::make_nvp("g", g) &
			boost::serialization::make_nvp("b", b);
	}
};

struct Vertex
{
	int		index;
	string	name;

	vector<Point>	points;

	Color	color;

	bool	valid;

	map<Point, int>	int_map;
	map<string, Point>	pt_map;

	float	fval;
	double	dval;

	Vertex()
		: index(1)
		, name("ver")
		, valid(false)
		, fval(120.0f)
		, dval(-112.998)
	{
	}

	bool operator == (Vertex const & rhs) const
	{
		return index == rhs.index && name == rhs.name && points == rhs.points &&
			color == rhs.color && valid == rhs.valid && int_map == rhs.int_map &&
			pt_map == rhs.pt_map && fval == rhs.fval && dval == rhs.dval;
	}

	template<class Archive>
	void	serialize(Archive & ar, unsigned int version)
	{
		BOOST_CHECK_EQUAL(version, 2);

		ar & index & name & points & color & valid & int_map & pt_map & fval & dval;
	}

};

BOOST_CLASS_VERSION(Color, 1)
BOOST_CLASS_VERSION(Vertex, 2)

int err_handler(lua_State* state)
{
	std::cerr << lua_tostring(state, -1);
	return 1;
}



BOOST_AUTO_TEST_CASE(test_lua_archive)
{
	ostringstream oss;
	LuaOArchive oa(oss);

	Vertex vertex;
	int num(100);

	boost::assign::push_back(vertex.points) (10, 20) (50, 60);
	boost::assign::insert(vertex.int_map) (Point(5, 6), 2) (Point(8, 9), 3);
	boost::assign::insert(vertex.pt_map) ("hello", Point(-1, -2)) ("world", Point(0, 111));
	vertex.color = Color(100, 128, 228);

	oa << boost::serialization::make_nvp("vertex", vertex) 
		<< boost::serialization::make_nvp("number", num);

	cout << oss.str();

	string expected = 
		"vertex = {\n"
		"\t__version = 2, 1, 'ver', \n"
		"\t{\n"
		"\t\t{10, 20}, \n"
		"\t\t{50, 60}\n"
		"\t}, \n"
		"\t{\n"
		"\t\t__version = 1, \n"
		"\t\tr = 100, \n"
		"\t\tg = 128, \n"
		"\t\tb = 228\n"
		"\t}, false, \n"
		"\t{\n"
		"\t\t[{5, 6}] = 2, \n"
		"\t\t[{8, 9}] = 3\n"
		"\t}, \n"
		"\t{\n"
		"\t\t['hello'] = {-1, -2}, \n"
		"\t\t['world'] = {0, 111}\n"
		"\t}, 120, -112.998\n"
		"}\n"
		"\n"
		"number = 100\n"
		"\n"
	;


	string const & ret = oss.str();

	// 便于找出哪里开始不同
	size_t size = std::min(ret.size(), expected.size());
	for(size_t i = 0; i < size; ++i)
	{
		BOOST_CHECK_EQUAL(ret[i], expected[i]);
	}

	BOOST_CHECK_EQUAL(ret.size(), expected.size());

	// 
	lua_State * state = lua_open();

	luaopen_base(state);
	luabind::open(state);
	luabind::set_pcall_callback(err_handler);

	lv::lua::dostr(state, ret.c_str());

	{
		LuaIArchive ia(luabind::globals(state));

		Vertex new_vertex;
		int new_num;

		ia >> boost::serialization::make_nvp("vertex", new_vertex) 
			>> boost::serialization::make_nvp("number", new_num);

		BOOST_CHECK(vertex == new_vertex);
		BOOST_CHECK_EQUAL(num, new_num);
	}
	

	lua_close(state);
}