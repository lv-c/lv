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
};

BOOST_AUTO_TEST_CASE(test_binary_stream)
{
	Buffer buf;
	OBufferStream os(buf);

	BinaryOStream bos(os);

	Test t;
	bos << t << int(1);
	
}