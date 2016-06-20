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

#include "MemoryAnalyser.h"

#include <lv/DataFlow/DataFlow.hpp>
#include <lv/DataFlow/Sink.hpp>
#include <lv/DataFlow/Source.hpp>

#include <lv/DataFlow/SyncPush.hpp>
#include <lv/DataFlow/ThreadedPush.hpp>

#include <lv/Serialization/IArchive.hpp>
#include <lv/Serialization/OArchive.hpp>

#include <lv/SimpleBufferManager.hpp>

#include <lv/Timer.hpp>

#include <string>

#include <Windows.h>

enum ParamType
{
	PT_Int,
	PT_String
};

typedef lv::IArchive iarchive_type;
typedef lv::OArchive oarchive_type;

using namespace std;

void pass_iarchive_obj(iarchive_type ia)
{

}

void echo(ParamType type, iarchive_type & ia)
{
	if (type == PT_Int)
	{
		int i;
		ia >> i;
		std::cout << "echo.PT_Int:" << i << std::endl;
	}
	else if (type == PT_String)
	{
		string s;
		ia >> s;
		std::cout << "echo.PT_String:" << s << std::endl;
	}
}

void hello()
{
	std::cout << "Hello World" << std::endl;
}

void sum(int a, float b)
{
	std::cout << "sum:" << (a + static_cast<int>(b)) << std::endl;
}


struct TestMemFn
{
	void test(int a, float b, float r) 
	{
		BOOST_CHECK_EQUAL(a + b, r);
	}

	void test_const(int a, float b, float r) const
	{
		BOOST_CHECK_EQUAL(a + b, r);
	}
};


// handle some exceptions
void proxy_push(lv::flow::slot_type const & slot, lv::ConstBufferRef const & buf)
{
	try
	{
		slot(buf);
	}
	catch (lv::flow::SerializationError const & ex)
	{
		std::cout << "flow::SerializationError: " << ex.what() << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(test_dataflow)
{
	lv::BufferManagerPtr buf_manager(new lv::SimpleBufferManager(1024));

	typedef std::string key_type;
	typedef std::string port_type;

	typedef lv::flow::DataFlow<lv::flow::ThreadedPush, port_type> dataflow_type;
	dataflow_type dataflow(5);	// number of threads

	typedef lv::flow::Source<key_type> source_type;
	source_type source(std::bind(&dataflow_type::push, &dataflow, port_type(), std::placeholders::_1), buf_manager);

	typedef lv::flow::Sink<lv::flow::SyncPush, key_type> sink_type;
	sink_type sink(&proxy_push);

	sink
		.reg("hello", &hello)
		.reg("sum", &sum)
		.reg("echo", &echo)
		.reg<void(float)>("sum_5", std::bind(&sum, 5, std::placeholders::_1))	// std::bind
		.reg_mem_fn("mem_fn", &TestMemFn::test, TestMemFn())
		.reg_mem_fn("const_mem_fn", &TestMemFn::test_const, TestMemFn())
		//.reg("pass_iarchive_obj", &pass_iarchive_obj)  // won't compile and shouldn't compile
	;

	

	lv::flow::Connection conn = dataflow.connect(port_type(), std::bind(&sink_type::push, &sink, std::placeholders::_1));

	source.call("hello");
	source.call("sum", 10, 40.3f);


	MemoryAnalyser::instance().attach();
	MemoryAnalyser::instance().begin_analyse();

	lv::Timer timer;

	// function object
	source.call("sum_5", 10.0f);

	source.call("mem_fn", 10, 5.0f, 15.0f);
	source.call("const_mem_fn", 10, 5.0f, 15.0f);

	double elapsed = timer.elapsed() * 1000;

	int counter;
	uint64 total_memory = MemoryAnalyser::instance().end_analyse(counter);
	MemoryAnalyser::instance().detach();

	cout << "Time elapsed:" << elapsed << endl;
	cout << "Total memory:" << total_memory << "  counter:" << counter << endl;


	

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