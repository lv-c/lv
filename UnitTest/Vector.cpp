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
#include <lv/IntType.hpp>
using namespace lv;

BOOST_AUTO_TEST_CASE(test_vector)
{
	Vector3f vecf1, vecf2;
	vecf1 = vecf2;		// OK

	VectorT<int16> vecs(vecf1);		// OK

	// vecf1 = vecs;	// shouldn't compile

	vecf1 = Vector3f(vecs);	// OK

}