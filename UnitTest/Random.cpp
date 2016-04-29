// *********************************************************************
//  Random   version:  1.0   ¡¤  date: 2011/06/15
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2011 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Random.hpp>

#include <iostream>

using namespace lv;
using namespace std;

BOOST_AUTO_TEST_CASE(test_random)
{
	Random<int>	rand_int;

	Random<float>	rand_float(-1.0f, 1.0f);

	for (size_t i = 0; i < 5; ++i)
	{
		float v = rand_float();
		cout << v << " ";

		BOOST_CHECK(rand_float.min() <= v && v <= rand_float.max());
	}

	cout << endl;
}