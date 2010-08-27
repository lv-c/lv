// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 08/25/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Graphics/Vector.hpp>
#include <lv/Graphics/Math.hpp>
#include <lv/IntType.hpp>
using namespace lv;

BOOST_AUTO_TEST_CASE(test_vector)
{
	{
		Vector3f vecf1, vecf2;
		vecf1 = vecf2;		// OK

		VectorT<int16> vecs(vecf1);		// OK

		// vecf1 = vecs;	// shouldn't compile

		vecf1 = Vector3f(vecs);	// OK

	}


	{
		Vector3f v(100.0f, 100.0f, 100.0f);
		BOOST_CHECK(std::abs(math::length(math::normalize(v)) - 1.0f) < 0.00001f);
	}

}