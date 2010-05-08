// *********************************************************************
//  Net   version:  1.0   ¡¤  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/FrameWork/Net/FlowSession.hpp>
#include <lv/FrameWork/Net/TcpSession.hpp>
#include <lv/FrameWork/Net/SSLServer.hpp>
#include <lv/FrameWork/Net/SSLSession.hpp>

#include <lv/SharedPtr.hpp>
#include <lv/SimpleBufferManager.hpp>


#ifndef _DEBUG
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#else
#pragma comment(lib, "libeay32d.lib")
#pragma comment(lib, "ssleay32d.lib")
#endif


using namespace lv::net;
using namespace lv;

#include <string>
using namespace std;

#include <boost/thread/condition.hpp>


namespace asio = boost::asio;

boost::condition g_condition_called;
boost::mutex	g_mutex;

template<template <class> class LowerSession>
class ClientSession : public FlowSession<string, LowerSession<ClientSide> >
{
	typedef FlowSession<string, LowerSession<ClientSide> > base_type;

	string	text_;

public:
	ClientSession(Context const & context)
		: base_type(context)
		, text_("Hello Net")
	{
		sink_
			.reg_mem_fn("notify", &ClientSession::notify, this)
		;
	}

private:

	virtual	void	on_connected()
	{
		source_->call("echo", text_);
	}

	void	notify(string const & str)
	{
		BOOST_CHECK_EQUAL(str, text_);

		std::cout << "notify:" << str << std::endl;

		boost::mutex::scoped_lock lock(g_mutex);
		g_condition_called.notify_all();
	}

};

template<template <class> class LowerSession>
class ServerSession : public FlowSession<string, LowerSession<ServerSide> >
{
	typedef FlowSession<string, LowerSession<ServerSide> > base_type;

public:
	ServerSession(Context const & context)
		: base_type(context)
	{
		sink_
			.reg_mem_fn("echo", &ServerSession::echo, this)
		;
	}

	void	echo(string const & str)
	{
		std::cout << str << std::endl;

		// call the notify function of the client session
		source_->call("notify", str);		
	}
};


template<class SessionType>
boost::shared_ptr<SessionType> create_session(Context const & context)
{
	return boost::shared_ptr<SessionType>(new SessionType (context));
}

template<template<class> class Server, template<class> class LowerSession>
void test_net_impl()
{
	typedef ServerSession<LowerSession>	server_session_type;
	typedef ClientSession<LowerSession> client_session_type;

	typedef Server<server_session_type>	server_type;

	asio::io_service service;

	Context context(lv::shared_new<SimpleBufferManager>(1024), lv::shared_from_object(service));

	if(boost::is_same<client_session_type, ClientSession<SSLSession> >::value)
	{
		context.create_ssl_context();
	}

	// server
	server_type server(context, 5555);

	server.start();

	// client. We must use shared_ptr here.
	boost::shared_ptr<client_session_type> client(new client_session_type(context));
	client->start("127.0.0.1", "5555");

	boost::mutex::scoped_lock lock(g_mutex);

	boost::thread thread(boost::bind(&asio::io_service::run, &service));

	// wait until it's finished
	g_condition_called.wait(lock);

	service.stop();
}

BOOST_AUTO_TEST_CASE(test_net)
{
	test_net_impl<ServerBase, TcpSession>();
	test_net_impl<SSLServer, SSLSession>();

}