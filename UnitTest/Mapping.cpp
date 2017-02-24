// *********************************************************************
//  Mapping   version:  1.0   ¡¤  date: 01/06/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Mapping.hpp>

using namespace lv;

#include <string>

using namespace std;


enum Language
{
	C,
	Cpp,
	Java,
	Php
};

BOOST_AUTO_TEST_CASE(test_mapping)
{
	{
		Mapping<Language, string> m = {
			{ C, "C" }, { Cpp, "Cpp" }, { Java, "Java" }
		};

		BOOST_CHECK_EQUAL(m.get_left(Cpp), "Cpp");
		BOOST_CHECK_EQUAL(m.get_right("Java"), Java);

		// shouldn't compile
		// m.begin()->second = "D";

		BOOST_CHECK_THROW(m.get_left(Php), mapping_key_not_found);
		
		BOOST_CHECK_EQUAL(m.get_left(Php, "Php"), "Php");

		BOOST_CHECK(m.get_right_optional("C"));
		BOOST_CHECK(! m.get_right_optional("A"));
	}

	{
		Mapping<int, short> m = {
			{ 10, 11 }, { 20, 21 }, { 0, 1 }
		};

		BOOST_CHECK_EQUAL(m.get_left(10), 11);
	}

	{
		Mapping<int, int> m = {
			{ 10, 11 }, { 20, 21 }, { 0, 1 }
		};

		BOOST_CHECK_EQUAL(m.get_left(10), 11);
		BOOST_CHECK_EQUAL(m.get_right(11), 10);
	}
}