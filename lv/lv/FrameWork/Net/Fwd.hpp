// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 01/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <memory>


namespace boost::asio
{
	class io_context;
}

namespace lv::net
{
	enum ErrorType
	{
		ErrorConnect,
		ErrorHandshake,
		ErrorRead,
		ErrorWrite
	};

	class Context;
	using ContextPtr = std::shared_ptr<Context>;

	class SocketHolder;
	using SocketHolderPtr = std::shared_ptr<SocketHolder>;

	class SessionBase;
	using SessionPtr = std::shared_ptr<SessionBase>;

	class TcpSession;

	template<class Key, class S>
	class FlowSession;

	class ISender;
	using ISenderPtr = std::shared_ptr<ISender>;

	class ServerBase;

	class SSLSession;
	
	class SSLServer;

	class SSLContext;

	class Socks5ClientSession;

	class Socks5ClientContext;

	class Socks5ServerSession;

	class Socks5ClientContext;

	class MessageQueue;

	class SteadyTimer;

	template<class SizeType>
	class PacketSplitter;


	namespace asio = boost::asio;

} 