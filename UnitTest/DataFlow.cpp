// *********************************************************************
//  DataFlow   version:  1.0   ��  date: 10/26/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS
#include "MemoryAnalyser.h"
#endif

#include <lv/DataFlow/DataFlow.hpp>
#include <lv/DataFlow/Sink.hpp>
#include <lv/DataFlow/Source.hpp>

#include <lv/Serialization/IArchive.hpp>
#include <lv/Serialization/OArchive.hpp>

#include <lv/SimpleBufferManager.hpp>

#include <lv/Timer.hpp>

#include <string>
#include <iostream>


enum ParamType
{
	PT_Int,
	PT_String
};

using iarchive_type = lv::IArchive;
using oarchive_type = lv::OArchive;

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
		std::cout << "echo.PT_Int: " << i << std::endl;
	}
	else if (type == PT_String)
	{
		string s;
		ia >> s;
		std::cout << "echo.PT_String: " << s << std::endl;
	}
}

void hello()
{
	std::cout << "Hello World" << std::endl;
}

void sum(int a, float b)
{
	std::cout << "sum: " << (a + static_cast<int>(b)) << std::endl;
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


BOOST_AUTO_TEST_CASE(test_dataflow)
{
	lv::BufferManagerPtr buf_manager = std::make_shared<lv::SimpleBufferManager>(1024);

	using key_type = std::string;
	using port_type = std::string;

	using dataflow_type = lv::flow::DataFlow<port_type>;
	dataflow_type dataflow;

	using source_type = lv::flow::Source<key_type>;
	source_type source([&dataflow](lv::BufferPtr buf) { dataflow.push(port_type(), buf); }, buf_manager);

	using sink_type = lv::flow::Sink<key_type>;
	sink_type sink;

	sink
		.reg("hello", &hello)
		.reg("sum", &sum)
		.reg("echo", &echo)
		.reg<void(float)>("sum_5", std::bind(&sum, 5, std::placeholders::_1))	// std::bind
		.reg_mem_fn("mem_fn", &TestMemFn::test, TestMemFn())
		.reg_mem_fn("const_mem_fn", &TestMemFn::test_const, TestMemFn())
		//.reg("pass_iarchive_obj", &pass_iarchive_obj)  // won't compile and shouldn't compile
	;

	
	lv::flow::Connection conn = dataflow.connect(port_type(), [&sink](lv::BufferPtr buf) {
		try {
			sink.push(*buf);
		}
		catch (lv::flow::SerializationError const & ex) {
			std::cout << "flow::SerializationError: " << ex.what() << std::endl;
		}
	});

	source.call("hello");
	source.call("sum", 10, 40.3f);


#ifdef LV_WIN32

	MemoryAnalyser::instance().attach();
	MemoryAnalyser::instance().begin_analyse();

#endif

	lv::Timer timer;

	// function object
	source.call("sum_5", 10.0f);

	source.call("mem_fn", 10, 5.0f, 15.0f);
	source.call("const_mem_fn", 10, 5.0f, 15.0f);

	double elapsed = timer.elapsed() * 1000;

#ifdef LV_WIN32

	int counter;
	uint64_t total_memory = MemoryAnalyser::instance().end_analyse(counter);
	MemoryAnalyser::instance().detach();

	cout << "Total memory: " << total_memory << "  counter: " << counter << endl;

#endif

	cout << "Time elapsed: " << elapsed << endl;


	

	// flow::SerializationError
	source.call("sum", 20);
	// flow::InvalidKey
	source.call("sum2", 20);

	// the target function use IArchve & as one of the parameters
	source.call("echo", PT_Int, 10);
	source.call("echo", PT_String, std::string("hello world"));

	// wait for the tasks to be finished
	this_thread::sleep_for(500ms);

	// disconnect
	BOOST_CHECK(conn.connected());
	conn.disconnect();
	BOOST_CHECK(!conn.connected());

	// it's disconnected and the function @a sum will not be called
	source.call("sum", 10, 20.0f);

}
