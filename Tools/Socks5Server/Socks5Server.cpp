#define _SCL_SECURE_NO_WARNINGS
#define BOOST_SP_USE_QUICK_ALLOCATOR

#pragma warning(disable : 4819)

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT	0x0501

#include <Windows.h>
#include <WinSock2.h>
#include <iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

#ifdef _DEBUG
#pragma comment(lib, "libluabindd.lib")
#else
#pragma comment(lib, "libluabind.lib")
#endif
#pragma comment(lib, "liblua.lib")


#include <vector>
using namespace std;

#include <lv/Log/Log.hpp>
#include <lv/Log/Utility.hpp>
#include <lv/Buffer.hpp>
#include <lv/SharedPtr.hpp>
#include <lv/SimpleBufferManager.hpp>
#include <lv/FrameWork/Net/ServerBase.hpp>
#include <lv/FrameWork/Net/Socks5ServerContext.hpp>
#include <lv/FrameWork/Net/Socks5ServerSession.hpp>
#include <lv/Toolkit/Lua/LuaConfig.hpp>
#include <lv/LuaArchive/Set.hpp>

using namespace lv;
using namespace lv::net;

Log	log_;

DWORD enumerate_ip(vector<string> & ip)
{
	ULONG buf_len(0);
	DWORD ret = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &buf_len);

	if(ret != ERROR_BUFFER_OVERFLOW)
		return ret;

	Buffer buf(buf_len);
	ret = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, (PIP_ADAPTER_ADDRESSES)buffer::data(buf), &buf_len);

	if(ret != ERROR_SUCCESS)
		return ret;

	PIP_ADAPTER_ADDRESSES cur_address = (PIP_ADAPTER_ADDRESSES)buffer::data(buf);
	while(cur_address)
	{
		PIP_ADAPTER_UNICAST_ADDRESS unicast = cur_address->FirstUnicastAddress;
		while(unicast != NULL)
		{
			char host[MAX_PATH] = {0};
			char service[MAX_PATH] = {0};

			ret = getnameinfo(unicast->Address.lpSockaddr, unicast->Address.iSockaddrLength, host, sizeof(host), 
				service, sizeof(service), NI_NUMERICHOST | NI_NUMERICSERV);

			if(ret != ERROR_SUCCESS)
				return ret;

			if(host != string("127.0.0.1"))
				ip.push_back(host);

			unicast = unicast->Next;
		}

		cur_address = cur_address->Next;
	}

	return 0;
}

namespace boost { namespace serialization {

	template<class Archive>
	void	serialize(Archive & ar, Socks5Auth & auth, unsigned int)
	{
		ar & auth.user & auth.password;
	}

} }

void on_error(ErrorType type, boost::system::error_code const & error)
{
	log_(lv::error) << net::error_string(type) << " : " << error.message();
}

typedef ServerBase	server_type;
typedef boost::shared_ptr<server_type>	server_ptr;

void on_new_session(SessionPtr session)
{
	log_() << "new connection:" << session->remote_ip();

	session->error_event().connect(on_error);
}


int main(int argc, char **argv)
{
	log::add_stdio_gather(log_);

	uint16 port;
	bool all_ip;
	bool emulate = false;

	if(argc >= 2 && argv[1] == string("-e"))
	{
		emulate = true;
		log_() << "emulate.";
	}

	set<Socks5Auth>	auth;

	try
	{
		LuaConfig cfg;
		cfg.load_file("config.lua");

		cfg ("start_port", port) ("all_ip", all_ip) ("auth", auth);
	}
	catch (std::exception const & ex)
	{
		log_(lv::error) << ex.what();
		return 1;
	}
	
	vector<string>	ip;

	if(all_ip)
	{
		DWORD ret = enumerate_ip(ip);

		if(ret != 0)
		{
			log_(lv::error) << "enumerate_ip failed:" << ret;
			return ret;
		}
	}
	else
	{
		ip.push_back(string());
	}


	boost::asio::io_service io;

	BufferManagerPtr buf_manager(new SimpleBufferManager(1024));

	vector<server_ptr>	servers;

	try
	{
		foreach(string const & v, ip)
		{
			boost::shared_ptr<Socks5ServerContext> context(new Socks5ServerContext(buf_manager, io));

			if(! v.empty())
			{
				context->address_to_bind(v);
			}

			context->set_auth(auth);

			log_() << "start server:" << v << " port:" << port;

			if(! emulate)
			{
				server_ptr server(new server_type(context, SessionCreator<Socks5ServerSession>()));

				server->new_session_event().connect(on_new_session);

				server->start(port);

				servers.push_back(server);
			}

			port++;
		}
	}
	catch (std::exception const & ex)
	{
		log_(lv::error) << ex.what();
		return 2;
	}
	
	
	boost::system::error_code error;
	io.run(error);

	return 0;
}

