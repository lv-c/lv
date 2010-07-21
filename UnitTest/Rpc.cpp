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

#define LV_RPC_DEBUG	0

#include <boost/tuple/tuple_comparison.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/comparison.hpp>

#include <lv/Packet/Tuple.hpp>
#include <lv/SimpleBufferManager.hpp>

#include <lv/rpc/Server.hpp>
#include <lv/rpc/Client.hpp>
#include <lv/rpc/Registery.hpp>
using namespace lv;
using namespace lv::rpc;

class MySocket : public ISocket
{
public:

	virtual	void	send(BufferPtr buf) 
	{
		/*
		cout << buf->size() << endl;
		for(size_t i = 0; i < buf->size(); ++i)
			cout << (int)(*buf)[i] << ' ';

		cout << endl;
		*/
	}
};

template<class S>
class ClientSocket : public MySocket
{
	typedef S server_t;
	server_t & server_;

	SocketPtr	svr_sock_;
public:
	ClientSocket(server_t * server, SocketPtr svr_sock) 
		: server_(*server) 
		, svr_sock_(svr_sock)
	{
	}

	virtual	void	send(BufferPtr buf) 
	{
		MySocket::send(buf);

		server_.on_receive(*buf, svr_sock_);
	}
};

template<class C>
class ServerSocket : public MySocket
{
	typedef C client_t;

public:
	client_t * client_;
	

	virtual void	send(BufferPtr buf)
	{
		MySocket::send(buf);

		client_->on_receive(*buf);
	}
};


class MyBufferManager : public IBufferManager
{
public:
	virtual BufferPtr	get()
	{
		return BufferPtr(new Buffer());
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


void another_add(int const & a, int b, int & result)
{
	result = a + b;
}

pair<string, int> test_pair(pair<string, int> const & p)
{
	return p;
}

boost::tuple<string, int, bool> test_tuple(boost::tuple<string, int, bool> const & t)
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

void throw_ex()
{
	throw std::runtime_error("hello wolrld");
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

class SingleObject : public Singleton<SingleObject, true>
{
	string str_;

public:

	SingleObject()
		: str_("single")
	{
	}

	string const & get() const
	{
		return str_;
	}
};

struct SOExtractor
{
	typedef lv::RefWrapper<SingleObject>	type;

	template<class IArchive>
	void operator () (IArchive & ia, type & re) const
	{
		re = boost::ref(SingleObject::instance());
	}
};

RPC_REGISTER_CLASS(Multiply)
RPC_REGISTER_CLASS(NonCopyable)

BOOST_AUTO_TEST_CASE(test_rpc)
{
	typedef Exceptions<PacketArchive, Protocol> excep_type;

	uint32 ex_seed = exception_hash_seed<Protocol::except_key_type>();

	boost::shared_ptr<excep_type> ex(new excep_type());

	typedef Registery<boost::fusion::map<boost::fusion::pair<SingleObject, SOExtractor> >, char const *>	registery_t;

	auto_ptr<registery_t> reg(new registery_t());
	(*reg)
		.reg("single.get", &SingleObject::get)
		.reg("add", add)			// register a free function
		.reg("another_add", another_add)
		.reg("mul", Multiply())		// register a function object
		.reg("mul_this", &Multiply::mul)	// register a member function
		.reg("throw_ex", throw_ex)
		.reg("echo", echo)
		.reg("test_pair", test_pair)
		.reg("test_tuple", test_tuple)
		// .reg("test_fusion_vector", test_fusion_vector)
		.reg("test_float", test_float)
		.reg("test_enum", test_enum)
	;

	typedef Server<registery_t> server_t;
	server_t server(BufferManagerPtr(new MyBufferManager()), reg);

	boost::shared_ptr<ServerSocket<Client<> > > ssock(new ServerSocket<Client<> >());
	Client<>	client(SocketPtr(new ClientSocket<server_t>(&server, ssock)), BufferManagerPtr(new SimpleBufferManager(100)), ex);

	ssock->client_ = &client;
	server.on_connected(ssock);

	// free function
	ReturningHandler<int> ret = client.call<int>("add", 1, 2);
	BOOST_CHECK_EQUAL(ret, 3);

	// float
	BOOST_CHECK_EQUAL(float(client.call<float>("test_float", 10.6f)), 10.6f);

	// enum
	BOOST_CHECK_EQUAL(EnumType(client.call<EnumType>("test_enum", ET_B)), ET_B);

	// member function
	BOOST_CHECK(int(client.call<int>("mul_this", -4, 5)) == -20);

	// function object
	BOOST_CHECK_EQUAL(int(client.call<int>("mul", 10000, 100000)), 1000000000);

	// reference.
	int result(-1);
	client.call<void>("another_add", 1, 4, result);
	BOOST_CHECK_EQUAL(result, -1);

	// throw exception
	Acknowledgment	ack = client.call<void>("throw_ex");
	BOOST_CHECK(ack.has_exception());
	BOOST_CHECK_THROW(ack(), std::runtime_error);

	// non-copyable parameter
	ReturningHandler<string> s_ret = client.call<string>("echo", NonCopyable("hello world"));
	BOOST_CHECK_EQUAL(string(s_ret), "hello world");

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

	// 
	BOOST_CHECK_THROW(ReturningHandler<string> wrong_returning_type = client.call<string>("add", 1, 2),
		boost::archive::archive_exception);
	// custom extractor
	BOOST_CHECK_EQUAL(string(client.call<string>("single.get")), "single");

	// test std::pair type
	typedef pair<string, int>	pair_type;
	BOOST_CHECK(pair_type(client.call<pair_type>("test_pair", make_pair(string("hello"), 100))) 
		== make_pair(string("hello"), 100));

	// test boost::tuple type
	typedef boost::tuple<string, int, bool> tuple_type;
	BOOST_CHECK(tuple_type(client.call<tuple_type>("test_tuple", boost::make_tuple(string("what"), 3, true))) 
		== boost::make_tuple(string("what"), 3, true));

	// test boost::fusion::vector type. fusion::vector is not supported. please use boost::tuple instead
	/*
	typedef boost::fusion::vector<int, string> fusion_vec_type;
	ReturningHandler<fusion_vec_type> fv_ret = client.call<fusion_vec_type>("test_fusion_vector", boost::fusion::make_vector(5, string("ok")));

	BOOST_CHECK(fv_ret() == boost::fusion::make_vector(5, string("ok")));
	*/
}