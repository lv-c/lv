// *********************************************************************
//  MemFn   version:  1.0   ¡¤  date: 12/28/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/MemFn.hpp>
#include <lv/SharedPtr.hpp>

#include <string>


struct Test
{
	std::string	test(int i, std::string * s, long const & l)
	{
		return *s;
	}

	std::string	test_const(int i, std::string * s, long const & l) const
	{
		return *s;
	}
};

template<typename F, typename T>
void test_bind(F f, T t)
{
	std::string str("aa");
	lv::result_of::bind_mem_fn<F>::type newfn = lv::bind_mem_fn(f, t);

	BOOST_CHECK_EQUAL(str, newfn(1, &str, 2));
}

BOOST_AUTO_TEST_CASE(test_memfn)
{
	Test t;

	// pointer
	test_bind(&Test::test, &t);
	test_bind(&Test::test_const, &t);

	// object
	test_bind(&Test::test, t);
	test_bind(&Test::test_const, t);

	// const object
	test_bind(&Test::test, Test());
	test_bind(&Test::test_const, Test());

	// ref
	test_bind(&Test::test, std::ref(t));
	test_bind(&Test::test_const, std::ref(t));

	// shared_ptr
	test_bind(&Test::test, lv::shared_from_object(t));
	test_bind(&Test::test_const, lv::shared_from_object(t));
}