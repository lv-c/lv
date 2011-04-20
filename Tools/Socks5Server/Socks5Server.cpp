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
#pragma comment(lib, "lua.lib")


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

typedef ServerBase<Socks5ServerSession>	server_type;
typedef boost::shared_ptr<server_type>	server_ptr;

void on_new_session(server_type::session_ptr session)
{
	log_() << "new connection:" << session->remote_ip();

	session->error_event().connect(on_error);
}

class MyBufferFactory : public Factory<Buffer>
{
	typedef public Factory<Buffer>	base_type;

	size_t	count_;

	size_t	max_count_;

public:

	MyBufferFactory(creator_type const & creator = NewCreator<Buffer>())
		: base_type(creator)
		, count_(0)
		, max_count_(0)
	{
	}

	virtual	shared_pointer	get()
	{
		++count_;

		if(count_ > max_count_)
		{
			max_count_ = count_;
			log_() << "current count:" << count_;
		}

		return base_type::get();
	}

private:

	virtual void	release(Buffer * obj)
	{
		--count_;

		base_type::release(obj);
	}

};


int main(int argc, char **argv)
{
	log::add_stdio_gather(log_);

	vector<string>	ip;
	DWORD ret = enumerate_ip(ip);

	if(ret != 0)
	{
		log_(lv::error) << "enumerate_ip failed:" << ret;
		return ret;
	}

	uint16 port;
	set<Socks5Auth>	auth;

	try
	{
		LuaConfig cfg;
		cfg.load_file("config.lua");

		cfg ("start_port", port) ("auth", auth);
	}
	catch (std::exception const & ex)
	{
		log_(lv::error) << ex.what();
		return 1;
	}
	

	boost::asio::io_service io;

	boost::shared_ptr<MyBufferFactory> factory(new MyBufferFactory());
	BufferManagerPtr buf_manager(new SimpleBufferManager(1024, factory));

	vector<server_ptr>	servers;

	try
	{
		foreach(string const & v, ip)
		{
			boost::shared_ptr<boost::asio::strand> strand(new boost::asio::strand(io));
			boost::shared_ptr<Socks5ServerContext> context(new Socks5ServerContext(buf_manager, strand));
			context->address_to_bind(boost::asio::ip::address::from_string(v));
			context->set_auth(auth);

			server_ptr server(new server_type(context));

			log_() << "start server:" << v << " port:" << port;

			server->new_session_event().connect(on_new_session);

			server->start(port);

			servers.push_back(server);

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

