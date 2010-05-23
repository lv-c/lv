// *********************************************************************
//  Serialization   version:  1.0   ·  date: 04/04/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Serialization/OArchive.hpp>
#include <lv/Serialization/IArchive.hpp>

#include <boost/array.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <vector>
#include <string>

using namespace std;
using namespace lv;

struct Test : boost::noncopyable
{
	int	a;
	string	s;

	Test()
		: a(0)
	{
	}

	Test(int a, string const &)
		: a(a)
		, s(s)
	{
	}

	bool operator == (Test const & rhs) const
	{
		return (a == rhs.a && s == rhs.s);
	}

	template<class Archive>
	void serialize(Archive & ar, unsigned int)
	{
		ar & a & s;
	}
};

struct CheckVersion
{
	template<class Archive>
	void serialize(Archive & ar, unsigned int version)
	{
		BOOST_CHECK_EQUAL(version, 2);
	}
};

BOOST_CLASS_VERSION(CheckVersion, 2)

BOOST_AUTO_TEST_CASE(test_serialization)
{

	Buffer  buf;
	buf.reserve(100);

	OBufferStream os(buf);
	OArchive oa(os);

	vector<string> str_vec;
	str_vec.push_back("hello");

	// 足够长的 string， 不会使用局部空间
	str_vec.push_back("ffffffffffffffffffffffffffffffffworld ffffffffffffffffffffffffffffffffffffffff");

	vector<int> int_vec(10, 20);
	Test obj(20, "hh");
	boost::array<int, 10> int_arr;
	int_arr.assign(20);
	CheckVersion ver;

	oa << int(10) << str_vec << int_vec << obj << int_arr << ver;

	IBufferStream is(buf);
	IArchive ia(is);

	int i = -1;
	vector<string> new_str_vec;
	vector<int> new_int_vec;
	Test new_obj;
	boost::array<int, 10> new_int_arr;
	CheckVersion new_ver;

	ia >> i >> new_str_vec >> new_int_vec >> new_obj >> new_int_arr >> new_ver;

	BOOST_CHECK_EQUAL(i, 10);
	BOOST_CHECK(str_vec == new_str_vec);
	BOOST_CHECK(int_vec == new_int_vec);
	BOOST_CHECK(obj == new_obj);
	BOOST_CHECK(int_arr == new_int_arr);

	// end of the buffer
	BOOST_CHECK_EQUAL(static_cast<size_t>(is.tellg()), buf.size());
}