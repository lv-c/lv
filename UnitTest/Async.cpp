// *********************************************************************
//  Async   version:  1.0   ¡¤  date: 2021/07/19
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2021 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Concurrent/Async.hpp>

#include <ranges>
#include <numeric>


BOOST_AUTO_TEST_CASE(test_async)
{
	// by value
	auto ret = lv::concurrent::fork_join(std::views::iota(0, 5), std::identity{});
	BOOST_CHECK_EQUAL(std::accumulate(ret.begin(), ret.end(), 0), 10);

	// by reference
	int arr[] = { 1 };
	lv::concurrent::fork_join(arr, [&](int & val) {
		BOOST_CHECK_EQUAL(&val, arr);
	});
}
