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

#include <lv/LuaArchive/Deque.hpp>
#include <lv/LuaArchive/Vector.hpp>
#include <lv/LuaArchive/LuaIArchive.hpp>
#include <lv/LuaArchive/LuaOArchive.hpp>
#include <lv/LuaArchive/PlainLuaIArchive.hpp>
#include <lv/LuaArchive/Map.hpp>
#include <lv/LuaArchive/DSTree.hpp>
#include <lv/LuaArchive/Mapping.hpp>
#include <lv/LuaArchive/Array.hpp>
#include <lv/LuaArchive/NonintrusiveOptional.hpp>
#include <lv/LuaArchive/BinaryObject.hpp>
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
		if (y == rhs.y)
			return x < rhs.x;
		else
			return y < rhs.y;
	}
};

struct Color
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	Color()
		: r(0)
		, g(0)
		, b(0)
	{
	}

	Color(uint8_t r, uint8_t g, uint8_t b)
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

enum Mode
{
	FullScreen	=	-1,
	Windowed	=	-2
};

BOOST_CLASS_VERSION(Color, 1)
BOOST_CLASS_VERSION(Vertex, 2)


int err_handler(lua_State * L)
{
	std::cerr << lua_tostring(L, -1);
	return 1;
}

BOOST_AUTO_TEST_CASE(test_lua_archive)
{
	ostringstream oss;
	LuaOArchive oa(oss, "\n\n");

	Vertex vertex;
	int num(100);
	Mode const mode(Windowed);
	DSTree<char, int> tree;
	DSTree<string, string>	tree2;
	deque<int>	que;
	que.push_back(123);
	Mapping<string, int> mapping;
	string mapping_key = "\\h'e\r\nll\to";
	mapping.insert(std::make_pair(mapping_key, 1));

	array<string, 3> arr = { "a", "bc" };

	boost::assign::push_back(vertex.points) (10, 20) (50, 60);
	boost::assign::insert(vertex.int_map) (Point(5, 6), 2) (Point(8, 9), 3);
	boost::assign::insert(vertex.pt_map) ("hello", Point(-1, -2)) ("world", Point(0, 111));
	vertex.color = Color(100, 128, 228);

	tree.insert((char const *)"hello", 10);
	tree.insert((char const *)"hem", 20);
	tree.insert((char const *)"what", 55);

	string const str1[] = {"aa", "ab", "fefe"};
	string const str2[] = {"mihn", "333"};
	string const str3[] = {"mihn", "xxxxx", "f", "3"};

	tree2.insert(str1, "hex");
	tree2.insert(str2, "xyy");
	tree2.insert(str3, shared_ptr<string>());

	uint64_t binary = 0x1234567887654321;

	oa << boost::serialization::make_nvp("vertex", vertex) 
		<< boost::serialization::make_nvp("number", num)
		<< boost::serialization::make_nvp("mode", mode)
		<< boost::serialization::make_nvp("que", que)
		<< boost::serialization::make_nvp("que_to_map", que)
		<< boost::serialization::make_nvp("mapping", mapping)
		<< boost::serialization::make_nvp("arr", arr)
		<< boost::serialization::make_nvp("binary", boost::serialization::make_binary_object(&binary, sizeof(binary)))
		<< boost::serialization::make_nvp("tree", tree)
		<< boost::serialization::make_nvp("tree2", tree2)
	;


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
		"mode = -2"
	;


	string const & ret = oss.str();

	// 便于找出哪里开始不同
	/*
	size_t size = std::min(ret.size(), expected.size());
	for (size_t i = 0; i < size; ++i)
	{
		BOOST_CHECK_EQUAL(ret[i], expected[i]);
	}

	BOOST_CHECK_EQUAL(ret.size(), expected.size());
	*/

	// 
	lua_State * L = luaL_newstate();

	luaL_openlibs(L);

	luabind::open(L);
	luabind::set_pcall_callback(err_handler);

	lv::lua::dostr(L, ret.c_str(), ret.size());

	{
		LuaIArchive ia(luabind::globals(L));
		
		Vertex new_vertex;
		int new_num;
		Mode new_mode;
		DSTree<char, int> new_tree;
		DSTree<string, string> new_tree2;
		deque<int> new_que;
		array<string, 3> new_arr;
		map<int, int> que_to_map;
		Mapping<string, int> new_mapping;
		auto optional_new_mapping = lv::serialization::make_optional(new_mapping);
		uint64_t new_binary;

		ia >> boost::serialization::make_nvp("vertex", new_vertex) 
			>> boost::serialization::make_nvp("number", new_num)
			>> boost::serialization::make_nvp("mode", new_mode)
			>> boost::serialization::make_nvp("que", new_que)
			>> boost::serialization::make_nvp("que_to_map", que_to_map)
			>> boost::serialization::make_nvp("mapping", optional_new_mapping)
			>> boost::serialization::make_nvp("arr", new_arr)
			>> boost::serialization::make_nvp("binary", boost::serialization::make_binary_object(&new_binary, sizeof(new_binary)))
			>> boost::serialization::make_nvp("tree", new_tree)
			>> boost::serialization::make_nvp("tree2", new_tree2)
		;

		BOOST_CHECK(vertex == new_vertex);
		BOOST_CHECK_EQUAL(num, new_num);
		BOOST_CHECK_EQUAL(mode, new_mode);
		BOOST_CHECK(tree == new_tree);
		BOOST_CHECK(tree2 == new_tree2);
		BOOST_CHECK(que == new_que);
		BOOST_CHECK(arr == new_arr);
		BOOST_CHECK_EQUAL(que_to_map[1], que[0]);
		BOOST_CHECK_EQUAL(new_mapping.size(), 1u);
		BOOST_CHECK_EQUAL(new_mapping.get_left(mapping_key), 1);
		BOOST_CHECK_EQUAL(binary, new_binary);
	}

	{
		PlainLuaIArchive ia(ret);
		
		Vertex new_vertex;
		int new_num;
		Mode new_mode;
		DSTree<char, int> new_tree;
		DSTree<string, string> new_tree2;
		deque<int> new_que;
		map<int, int> que_to_map;
		Mapping<string, int> new_mapping;
		auto optional_new_mapping = lv::serialization::make_optional(new_mapping);
		array<string, 3> new_arr;
		uint64_t new_binary;

		ia >> boost::serialization::make_nvp("vertex", new_vertex) 
			>> boost::serialization::make_nvp("number", new_num)
			>> boost::serialization::make_nvp("mode", new_mode)
			>> boost::serialization::make_nvp("que", new_que)
			>> boost::serialization::make_nvp("que_to_map", que_to_map)
			>> boost::serialization::make_nvp("mapping", optional_new_mapping)
			>> boost::serialization::make_nvp("arr", new_arr)
			>> boost::serialization::make_nvp("binary", boost::serialization::make_binary_object(&new_binary, sizeof(new_binary)))
			// >> boost::serialization::make_nvp("tree", new_tree)
			// >> boost::serialization::make_nvp("tree2", new_tree2)
		;

		BOOST_CHECK(vertex == new_vertex);
		BOOST_CHECK_EQUAL(num, new_num);
		BOOST_CHECK_EQUAL(mode, new_mode);
		BOOST_CHECK(que == new_que);
		BOOST_CHECK_EQUAL(que_to_map[1], que[0]);
		BOOST_CHECK_EQUAL(new_mapping.size(), 1u);
		BOOST_CHECK_EQUAL(new_mapping.get_left(mapping_key), 1);
		BOOST_CHECK(arr == new_arr);
		// BOOST_CHECK(tree == new_tree);
		// BOOST_CHECK(tree2 == new_tree2);
	}
	

	lua_close(L);
}
