#include "stdafx.h"
#include "RepeaterSession.hpp"

#include <lv/FrameWork/Net/ServerBase.hpp>
#include <lv/FrameWork/Net/Context.hpp>
#include <lv/Toolkit/Lua/LuaConfig.hpp>
#include <lv/LuaArchive/Map.hpp>
#include <lv/SimpleBufferManager.hpp>
#include <lv/Log/Utility.hpp>


#include <boost/asio/io_service.hpp>


struct Address
{
	string	ip;
	uint16	port;

	template<class Archive>
	void	serialize(Archive & ar, unsigned int)
	{
		ar & ip & port;
	}
};

class RepeaterSessionCreater
{
	string	ip_;

	string	port_;

public:

	RepeaterSessionCreater(string const & ip, string const & port)
		: ip_(ip)
		, port_(port)
	{
	}

	SessionPtr operator () (ContextPtr context) const
	{
		return SessionPtr(new RepeaterSession(context, ip_, port_));
	}
};


int main(int argc, char **argv)
{
	lv::log::add_stdio_gather(LOG);

	// config
	typedef map<uint16, Address> AddressMap;
	AddressMap mapping;

	try
	{
		LuaConfig cfg;
		cfg.load_file("config.lua");

		cfg ("mapping", mapping);
	}
	catch(exception const & ex)
	{
		LOG(lv::error) << ex.what();
		return 1;
	}


	// context
	boost::asio::io_service service;
	BufferManagerPtr buf_manager(new SimpleBufferManager(1024));

	ContextPtr context(new Context(buf_manager, service));

	typedef boost::shared_ptr<ServerBase> ServerPtr;

	vector<ServerPtr> servers;

	foreach(AddressMap::const_reference v, mapping)
	{
		RepeaterSessionCreater creater(v.second.ip, boost::lexical_cast<string>(v.second.port));
		ServerPtr server(new ServerBase(context, creater));

		server->start(v.first);

		servers.push_back(server);

		LOG() << v.first << " -> " << v.second.ip << ":" << v.second.port;
	}

	// run
	boost::system::error_code error;
	service.run(error);

	return 0;
}