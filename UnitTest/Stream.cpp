// *********************************************************************
//  Stream   version:  1.0   ¡¤  date: 04/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Stream/StreamBase.hpp>
#include <lv/Stream/IBufferStream.hpp>
#include <lv/Stream/OBufferStream.hpp>

using namespace lv;

BOOST_AUTO_TEST_CASE(test_stream_base)
{
	StreamBase stream_base;
}


BOOST_AUTO_TEST_CASE(test_stream)
{
	Buffer buf;

	OBufferStream os(buf);

	char str[] = "hello world";
	size_t const size = sizeof(str);
	os.write(str, size);

	BOOST_CHECK_EQUAL(buf.size(), size);
	BOOST_CHECK_EQUAL(memcmp(str, &buf[0], size), 0);

	IBufferStream is(buf);
	char new_str[size];
	is.read(new_str, size);

	BOOST_CHECK_EQUAL(memcmp(str, new_str, size), 0);

	is.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	BOOST_CHECK_THROW(is.read(new_str, 1), std::ios_base::failure);
}