// *********************************************************************
//  ZeroMQ   version:  1.0   ¡¤  date: 2012/03/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/ZeroMQ/SinkSocket.hpp>
#include <lv/ZeroMQ/SourceSocket.hpp>
#include <lv/ZeroMQ/Device.hpp>
#include <lv/Serialization/OArchive.hpp>
#include <lv/Serialization/IArchive.hpp>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>


#ifndef _DEBUG
#pragma comment(lib, "libzmq.lib")
#else
#pragma comment(lib, "libzmqD.lib")
#endif

zmq::context_t context(1);
bool	hello_called = false;
boost::condition	source_init;
boost::asio::io_service	service;

using namespace lv;
using namespace lv::zeromq;

void	source_proc()
{
	SourceSocket<> source(context, ZMQ_PUB);

	source.bind("inproc://test");

	SinkSocket<> sink(context, ZMQ_SUB, service);
	sink.connect("inproc://test");

	SourceSocket<> front_source(context, ZMQ_PUB);

	front_source.bind("inproc://front");

	Device device;
	device.start(sink, front_source);

	boost::thread thread(boost::bind(&boost::asio::io_service::run, &service));
	source_init.notify_all();


	for(size_t i = 0; i < 2; ++i)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(200));
		source.call("hello_world");
	}
}

void	hello_world()
{
	hello_called = true;
}

void	sink_proc()
{
	SinkSocket<> sink(context, ZMQ_SUB, service);

	sink.reg("hello_world", &hello_world);

	boost::mutex mutex;
	boost::mutex::scoped_lock lock(mutex);
	source_init.wait(lock);

	sink.connect("inproc://front");

	while(! hello_called)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(1));
	}
}

BOOST_AUTO_TEST_CASE(test_zeromq)
{
	boost::thread_group group;
	group.create_thread(source_proc);
	group.create_thread(sink_proc);

	group.join_all();

	BOOST_CHECK(hello_called);
}