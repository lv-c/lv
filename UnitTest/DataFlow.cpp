// *********************************************************************
//  DataFlow   version:  1.0   ¡¤  date: 10/26/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/DataFlow/DataFlow.hpp>
#include <lv/DataFlow/Sink.hpp>
#include <lv/DataFlow/Source.hpp>

#include <lv/DataFlow/SyncPush.hpp>
#include <lv/DataFlow/ThreadedPush.hpp>

#include <lv/Packet/IPacket.hpp>
#include <lv/Packet/OPacket.hpp>

#include <lv/SimpleBufferManager.hpp>

#include <string>

#include <Windows.h>

enum ParamType
{
	PT_Int,
	PT_String
};

void pass_iarchive_obj(lv::IPacket ia)
{

}

void echo(ParamType type, lv::IPacket & ia)
{
	if(type == PT_Int)
	{
		int i;
		ia >> i;
		std::cout << "echo.PT_Int:" << i << std::endl;
	}
	else if(type == PT_String)
	{
		string s;
		ia >> s;
		std::cout << "echo.PT_String:" << s << std::endl;
	}
}

void sum(int a, float b)
{
	std::cout << "sum:" << (a + static_cast<int>(b)) << std::endl;
}


// handle some exceptions
void proxy_push(lv::flow::slot_type const & slot, lv::BufferPtr buf)
{
	try
	{
		slot(buf);
	}
	catch (lv::flow::SerializationError const & ex)
	{
		std::cout << "flow::SerializationError: " << ex.what() << std::endl;
	}
	catch(lv::flow::InvalidKey const & ex)
	{
		std::cout << "flow::InvalidKey: " << ex.what() << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(test_dataflow)
{
	lv::BufferManagerPtr buf_manager(new lv::SimpleBufferManager(1024));

	typedef std::string key_type;
	typedef std::string port_type;

	typedef lv::flow::DataFlow<lv::flow::ThreadedPush, port_type> dataflow_type;
	dataflow_type dataflow(dataflow_type::push_policy_type(5));	// number of threads

	typedef lv::flow::Source<key_type, port_type> source_type;
	source_type source(port_type(), boost::bind(&dataflow_type::push, &dataflow, _1, _2), buf_manager);

	typedef lv::flow::Registery<key_type> registery_type;
	std::auto_ptr<registery_type> reg(new registery_type());

	(*reg)
		.reg("sum", &sum)
		.reg("echo", &echo)
		//.reg("pass_iarchive_obj", &pass_iarchive_obj)  // won't compile and shouldn't compile
	;

	typedef lv::flow::Sink<lv::flow::SyncPush, key_type> sink_type;
	sink_type sink(reg, &proxy_push);

	lv::flow::Connection conn = dataflow.connect(port_type(), boost::bind(&sink_type::push, &sink, _1));

	// streaming
	source.stream("sum") << 10 << 40.3f;

	// flow::SerializationError
	source.call("sum", 20);
	// flow::InvalidKey
	source.call("sum2", 20);

	// the target function use IArchve & as one of the parameters
	source.call("echo", PT_Int, 10);
	source.call("echo", PT_String, std::string("hello world"));

	// wait for the tasks to be finished
	::Sleep(500);

	// disconnect
	BOOST_CHECK(conn.connected());
	conn.disconnect();
	BOOST_CHECK(! conn.connected());

	// it's diconnected and the function @a sum will not be called
	source.call("sum", 10, 20.0f);

}