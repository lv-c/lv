// *********************************************************************
//  DSTree   version:  1.0   ¡¤  date: 12/04/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Algorithm/DSTree.hpp>
#include <lv/Serialization/DSTree.hpp>

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

typedef lv::DSTree<char, int>	dstree_type;

void check_serialize_dstree(dstree_type const & tree)
{
	std::ostringstream oss;
	boost::archive::text_oarchive oa(oss);
	oa << tree;

	std::cout << oss.str() << std::endl;

	std::istringstream iss(oss.str());
	boost::archive::text_iarchive ia(iss);

	dstree_type new_tree;
	ia >> new_tree;

	BOOST_CHECK(tree == new_tree);
}

BOOST_AUTO_TEST_CASE(test_dstree)
{

	dstree_type a, b;

	check_serialize_dstree(a);

	BOOST_CHECK(a == b);

	a.insert((char const*)"hello", 10);
	BOOST_CHECK(a != b);

	check_serialize_dstree(a);

	b.insert((char const*)"hello", 10);
	BOOST_CHECK(a == b);

	a.insert((char const*)"what???", 40);
	BOOST_CHECK(a != b);

	check_serialize_dstree(a);
}
