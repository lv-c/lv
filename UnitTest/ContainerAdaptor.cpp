// *********************************************************************
//  ContainerAdaptor   version:  1.0   ¡¤  date: 2019/01/08
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/ContainerAdaptor/Operations.hpp>
#include <lv/ContainerAdaptor/Vector.hpp>
#include <lv/ContainerAdaptor/List.hpp>
#include <lv/ContainerAdaptor/Deque.hpp>
#include <lv/ContainerAdaptor/String.hpp>
#include <lv/ContainerAdaptor/Map.hpp>
#include <lv/ContainerAdaptor/Mapping.hpp>
#include <lv/ContainerAdaptor/Set.hpp>
#include <lv/ContainerAdaptor/Interprocess/Vector.hpp>


template<class T>
void	test_container_operations()
{
	static_assert(lv::container_adaptor::is_container_v<T>);

	using value_type = typename T::value_type;

	T t;

	lv::clear(t);
	lv::push(t, value_type{});

	BOOST_CHECK_EQUAL(std::size(t), 1);
}


BOOST_AUTO_TEST_CASE(test_container_adaptor)
{
	static_assert(!lv::container_adaptor::is_container_v<int>);

	test_container_operations<std::vector<int> >();
	test_container_operations<std::list<int> >();
	test_container_operations<std::deque<int> >();

	test_container_operations<std::string>();

	test_container_operations<std::map<int, int> >();
	test_container_operations<lv::Mapping<int, int> >();
	test_container_operations<std::set<int> >();

	test_container_operations<boost::interprocess::vector<int> >();
}

