// *********************************************************************
//  Net   version:  1.0   ��  date: 01/03/2010
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
#include <lv/FrameWork/Net/SSLContext.hpp>

#include <lv/SharedPtr.hpp>

#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

#ifdef LV_PLATFORM_WINDOWS
#	pragma comment(lib, "libcrypto.lib")
#	pragma comment(lib, "libssl.lib")
#	pragma comment(lib, "CRYPT32.LIB")
#endif


using namespace lv::net;
using namespace lv;

#include <string>
using namespace std;


condition_variable g_condition_called;
mutex	g_mutex;

template<class LowerSession>
class ClientSession : public FlowSession<string, LowerSession>
{
	using base_type = FlowSession<string, LowerSession>;

	string	text_;

public:
	ClientSession(ContextPtr context)
		: base_type(context)
		, text_("Hello Net")
	{
		this->sink_
			.reg_mem_fn("notify", &ClientSession::notify, this)
		;
	}

private:

	void	on_connected() override
	{
		this->source_.call("echo", text_);
	}

	void	on_error(ErrorType type, boost::system::error_code const & error) override
	{
		std::cout << "client err: " << error.message() << std::endl;
	}

	void	notify(string const & str)
	{
		BOOST_CHECK_EQUAL(str, text_);

		std::cout << "notify: " << str << std::endl;

		std::lock_guard<mutex> lock(g_mutex);
		g_condition_called.notify_all();
	}

};

template<class LowerSession>
class ServerSession : public FlowSession<string, LowerSession>
{
	using base_type = FlowSession<string, LowerSession>;

public:
	ServerSession(ContextPtr context)
		: base_type(context)
	{
		this->sink_
			.reg_mem_fn("echo", &ServerSession::echo, this)
		;
	}

	void	echo(string const & str)
	{
		std::cout << str << std::endl;

		// call the notify function of the client session
		this->source_.call("notify", str);		
	}

private:

	void	on_error(ErrorType type, boost::system::error_code const & error) override
	{
		std::cout << "server err: " << error.message() << std::endl;
	}
};

template<class Server, class LowerSession>
void test_net_impl()
{
	using server_session_type = ServerSession<LowerSession>;
	using client_session_type = ClientSession<LowerSession>;

	using server_type = Server;

	boost::asio::io_context io;

	// 
	shared_ptr<SSLContext> server_context = std::make_shared<SSLContext>(io);
	shared_ptr<SSLContext> client_context = std::make_shared<SSLContext>(io);

	// server
	server_type server(server_context, SessionCreator<server_session_type>());

	server.start(5555);

	// client. We must use shared_ptr here.
	shared_ptr<client_session_type> client = std::make_shared<client_session_type>(client_context);
	client->connect("127.0.0.1", "5555");

	unique_lock<mutex> lock(g_mutex);

	std::thread thread([&io]() { io.run(); });

	// wait until it's finished
	g_condition_called.wait(lock);

	io.stop();

	thread.join();
}

BOOST_AUTO_TEST_CASE(test_net)
{
	test_net_impl<ServerBase, TcpSession>();

	// broken. unknown bytes received after handshake
	// test_net_impl<SSLServer, SSLSession>();

	// ENGINE_cleanup();
}
