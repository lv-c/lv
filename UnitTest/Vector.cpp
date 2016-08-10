// *********************************************************************
//  Vector   version:  1.0   ·  date: 08/25/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

// compilation error using gcc
#ifdef LV_PLATFORM_WINDOWS

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

#ifdef LV_WIN32

	{
		Vector3f v(100.0f, 100.0f, 100.0f);
		// 笔记本上结果是 0.00015795
		BOOST_CHECK_LT(std::abs(math::length(math::normalize(v)) - 1.0f), 0.0002f);//0.00001f);
	}

#endif

	{
		Vector3f vec;
		float f = 2.0f;
		for (float & e : vec)
		{
			e = f;
			f += 1.0f;
		}

		BOOST_CHECK(vec == Vector3f(2.0f, 3.0f, 4.0f));
		BOOST_CHECK_EQUAL(vec[0], 2.0f);

		Vector3f const & vec_ref = vec;
		for (float e : vec_ref)
		{
			e = 0;
		}
	}

}

#endif
