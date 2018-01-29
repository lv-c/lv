// *********************************************************************
//  Rpc   version:  1.0   ¡¤  date: 11/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************


#include "UnitTest.hpp"

#include <iostream>
using namespace std;

#include <lv/SharedPtr.hpp>
#include <lv/Buffer.hpp>
#include <lv/Singleton.hpp>

#include <boost/tuple/tuple_comparison.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/comparison.hpp>

#include <lv/Serialization/Tuple.hpp>
#include <lv/SimpleBufferManager.hpp>

#include <lv/RPC/Server.hpp>
#include <lv/RPC/Client.hpp>
#include <lv/RPC/Registery.hpp>
#include <lv/RPC/PacketArchive.hpp>

using namespace lv;
using namespace lv::rpc;


class MyBufferManager : public IBufferManager
{
public:

	BufferPtr	get() override
	{
		return std::make_shared<Buffer>();
	}
};

int add(int a, int const & b)
{
	return a + b;
}

class NonCopyable : boost::noncopyable
{
	string	str_;
public:

	// we must have a default constructor
	NonCopyable()
	{
	}

	NonCopyable(string const & str) : str_(str) {}

	string const & str() const { return str_; }

	template<class Archive>
	void serialize(Archive & ar, const unsigned int )
	{
		ar & str_;
	}
};

string echo(NonCopyable const & obj)
{
	return obj.str();
}

pair<string, int> test_pair(pair<string, int> const & p)
{
	return p;
}

tuple<string, int, bool> test_tuple(tuple<string, int, bool> const & t)
{
	return t;
}

boost::fusion::vector<int, string> test_fusion_vector(boost::fusion::vector<int, string> const & v)
{
	return v;
}

float test_float(float f)
{
	return f;
}

enum EnumType
{
	ET_A,
	ET_B,
	ET_C
};

EnumType test_enum(EnumType e)
{
	return e;
}

struct Multiply
{
	int mul(int a, int b) const
	{
		return a * b;
	}

	int operator () (int a, int b) const
	{
		return a * b;
	}

	template<class Archive>
	void serialize(Archive & ar, const unsigned int )
	{

	}
};

using server_t = Server<>;

struct Tunnel
{
	Client<> *	client;

	server_t *	server;

	void operator ()(BufferPtr buf)
	{
		BufferPtr reply = server->on_receive(buf);
		if (reply)
		{
			client->on_receive(reply);
		}
	}
};


RPC_REGISTER_CLASS(Multiply)
RPC_REGISTER_CLASS(NonCopyable)

BOOST_AUTO_TEST_CASE(test_rpc)
{
	server_t server(std::make_shared<MyBufferManager>());

	Multiply multiply;

	server
		.reg("add", add)			// register a free function
		.reg("mul", Multiply())		// register a function object
		.reg_mem_fn("mul_this", &Multiply::mul, &multiply)	// register a member function
		.reg("echo", echo)
		.reg("test_pair", test_pair)
		.reg("test_tuple", test_tuple)
		// .reg("test_fusion_vector", test_fusion_vector)
		.reg("test_float", test_float)
		.reg("test_enum", test_enum)
	;


	Tunnel tunnel;
	Client<> client(std::ref(tunnel), std::make_shared<SimpleBufferManager>(100));

	tunnel.client = &client;
	tunnel.server = &server;

	// void
	Acknowledgment ack = client.call<void>("add", 0, 1);
	ack.get();

	// free function
	Future<int> ret = client.call<int>("add", 1, 2);
	BOOST_CHECK_EQUAL(ret.get(), 3);

	// float
	BOOST_CHECK_EQUAL(float(client.call<float>("test_float", 10.6f)), 10.6f);

	// enum
	BOOST_CHECK_EQUAL(EnumType(client.call<EnumType>("test_enum", ET_B)), ET_B);

	// member function
	BOOST_CHECK(int(client.call<int>("mul_this", -4, 5)) == -20);

	// function object
	BOOST_CHECK_EQUAL(int(client.call<int>("mul", 10000, 100000)), 1000000000);

	// non-copyable parameter
	Future<string> s_ret = client.call<string>("echo", NonCopyable("hello world"));
	BOOST_CHECK_EQUAL(s_ret.get(), "hello world");

	/* Should catch from the server side

	// not existed function 
	BOOST_CHECK_THROW(client.call<int>("not_existed_function"), InvalidFunctionID);

	// unmatched parameter type
	BOOST_CHECK_THROW(client.call<int>("add", static_cast<short>(1), 2), SerializationError);

	try
	{
		client.call<int>("add", static_cast<short>(1), 2);
	}
	catch (SerializationError const & ex)
	{
		std::cout << ex.what() << std::endl;
	}

	*/

	// 
	BOOST_CHECK_THROW(Future<string> wrong_returning_type = client.call<string>("add", 1, 2),
		boost::archive::archive_exception);

	// test std::pair type
	using pair_type = pair<string, int>;
	BOOST_CHECK(pair_type(client.call<pair_type>("test_pair", make_pair(string("hello"), 100))) 
		== make_pair(string("hello"), 100));

	// test std::tuple type
	using tuple_type = tuple<string, int, bool>;
	BOOST_CHECK(tuple_type(client.call<tuple_type>("test_tuple", std::make_tuple(string("what"), 3, true))) 
		== std::make_tuple(string("what"), 3, true));

	// test boost::fusion::vector type. fusion::vector is not supported. please use std::tuple instead
	/*
	using fusion_vec_type = boost::fusion::vector<int, string>;
	Future<fusion_vec_type> fv_ret = client.call<fusion_vec_type>("test_fusion_vector", boost::fusion::make_vector(5, string("ok")));

	BOOST_CHECK(fv_ret() == boost::fusion::make_vector(5, string("ok")));
	*/
}