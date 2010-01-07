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

#include <lv/FrameWork/Net/ServerBase.hpp>
#include <lv/FrameWork/Net/FlowSession.hpp>
#include <lv/FrameWork/Net/TcpSession.hpp>

#include <lv/SharedPtr.hpp>
#include <lv/SimpleBufferManager.hpp>


using namespace lv::net;
using namespace lv;

#include <string>
using namespace std;

#include <boost/thread/condition.hpp>


namespace asio = boost::asio;

boost::condition g_condition_called;
boost::mutex	g_mutex;

class ClientSession : public FlowSession<string, TcpSession<ClientSide> >
{
	typedef FlowSession<string, TcpSession<ClientSide> > base_type;

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

class ServerSession : public FlowSession<string, TcpSession<ServerSide> >
{
	typedef FlowSession<string, TcpSession<ServerSide> > base_type;

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


boost::shared_ptr<ServerSession> create_session(Context const & context)
{
	return boost::shared_ptr<ServerSession>(new ServerSession(context));
}

BOOST_AUTO_TEST_CASE(test_net)
{

	asio::io_service service;

	Context context(BufferManagerPtr(new SimpleBufferManager(1024)), lv::shared_from_object(service));

	// server
	typedef ServerBase<ServerSession> server_type;
	server_type server(service, 5555, boost::bind(create_session, context));

	server.start();

	// client. We must use shared_ptr here.
	boost::shared_ptr<ClientSession> client(new ClientSession(context));
	client->start("127.0.0.1", "5555");

	boost::mutex::scoped_lock lock(g_mutex);

	boost::thread thread(boost::bind(&asio::io_service::run, &service));

	// wait until it's finished
	g_condition_called.wait(lock);

	service.stop();
}