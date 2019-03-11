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
#include <lv/BinaryStream/Array.hpp>
#include <lv/BinaryStream/Vector.hpp>
#include <lv/BinaryStream/String.hpp>
#include <lv/BinaryStream/Buffer.hpp>
#include <lv/BinaryStream/Interprocess/String.hpp>

using namespace lv;


struct Test
{
	int	i;

	template<class Archive>
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
	static_assert(std::is_base_of_v<bstream::primitive_buffer_tag, bstream::object_tag<std::array<int, 1> > >);
	static_assert(!std::is_base_of_v<bstream::primitive_buffer_tag, bstream::object_tag<std::array<std::string, 1> > >);

	static_assert(std::is_base_of_v<bstream::primitive_buffer_tag, bstream::object_tag<std::vector<int> > >);
	static_assert(std::is_base_of_v<bstream::primitive_buffer_tag, bstream::object_tag<Buffer> >);
	static_assert(std::is_base_of_v<bstream::primitive_buffer_tag, bstream::object_tag<std::string> >);

	static_assert(std::is_base_of_v<bstream::primitive_buffer_tag, bstream::object_tag<boost::interprocess::string> >);

	static_assert(!std::is_base_of_v<bstream::primitive_buffer_tag, bstream::object_tag<std::vector<std::string> > >);
	static_assert(std::is_base_of_v<bstream::range_tag, bstream::object_tag<std::vector<std::string> > >);


	test_binary_stream_impl(true);
	test_binary_stream_impl(false);
}
