// *********************************************************************
//  BinaryStream   version:  1.0   ¡¤  date: 04/06/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/BinaryStream.hpp>

using namespace lv;

struct Test
{
	int	i;

	template<typename Archive>
	void serialize(Archive & ar, unsigned int)
	{
		ar & i;
	}

	bool operator == (Test const & rhs)
	{
		return i == rhs.i;
	}
};

void test_binary_stream_impl(bool switch_endian)
{
	Buffer buf;
	BinaryOStream bos(buf);
	
	bos.switch_endian(switch_endian);

	Test t{};
	int i(0x123455);
	bos << t << i;

	//

	BinaryIStream bis(buf);

	bis.switch_endian(switch_endian);

	Test new_t;
	int new_i;
	bis >> new_t >> new_i;

	BOOST_CHECK(new_t == t);
	BOOST_CHECK_EQUAL(new_i, i);
}

BOOST_AUTO_TEST_CASE(test_binary_stream)
{
	test_binary_stream_impl(true);
	test_binary_stream_impl(false);
}
