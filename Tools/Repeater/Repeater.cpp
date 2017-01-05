#include "stdafx.h"
#include "RepeaterSession.hpp"
#include "Monitor.hpp"
#include "Config.hpp"

#include <lv/FrameWork/Net/ServerBase.hpp>
#include <lv/FrameWork/Net/Context.hpp>
#include <lv/Toolkit/Lua/LuaConfig.hpp>
#include <lv/LuaArchive/Map.hpp>
#include <lv/SimpleBufferManager.hpp>
#include <lv/Log/Utility.hpp>


#include <boost/asio/io_service.hpp>



class RepeaterSessionCreater
{
	string	ip_;

	string	port_;

	Monitor &	monitor_;

public:

	RepeaterSessionCreater(string const & ip, string const & port, Monitor & monitor)
		: ip_(ip)
		, port_(port)
		, monitor_(monitor)
	{
	}

	SessionPtr operator () (ContextPtr context) const
	{
		return std::make_shared<RepeaterSession>(context, ip_, port_, monitor_);
	}
};

// TODO : refuse a connection if we can't connect to the destination. WSAAccept ?


int main(int argc, char **argv)
{
	lv::log::add_stdio_gather(LOG);

	// config
	Config cfg;

	try
	{
		LuaConfig lua_cfg;
		lua_cfg.load_file("config.lua");

		lua_cfg (cfg);
	}
	catch (exception const & ex)
	{
		LOG(lv::error) << ex.what();
		return 1;
	}


	// context
	boost::asio::io_service service;
	BufferManagerPtr buf_manager(new SimpleBufferManager(1024));

	ContextPtr context = std::make_shared<Context>(buf_manager, service);

	typedef unique_ptr<ServerBase> ServerPtr;

	vector<ServerPtr> servers;

	Monitor stat(service);

	for (auto const & v : cfg.mapping)
	{
		RepeaterSessionCreater creater(v.second.ip, boost::lexical_cast<string>(v.second.port), stat);
		ServerPtr server = std::make_unique<ServerBase>(context, creater);

		server->start(v.first);

		servers.push_back(std::move(server));

		LOG() << v.first << " -> " << v.second.ip << ":" << v.second.port;
	}

	// run
	boost::system::error_code error;
	service.run(error);

	return 0;
}