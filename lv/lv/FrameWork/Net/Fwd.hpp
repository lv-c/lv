// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 01/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_FWD_HPP
#define LV_NET_FWD_HPP

#include <memory>


namespace boost { namespace asio {} }

namespace lv { namespace net {

	enum ErrorType
	{
		ErrorConnect,
		ErrorHandshake,
		ErrorRead,
		ErrorWrite
	};

	class Context;
	typedef std::shared_ptr<Context>	ContextPtr;

	class SocketHolder;
	typedef std::shared_ptr<SocketHolder>	SocketHolderPtr;

	class SessionBase;
	typedef std::shared_ptr<SessionBase>	SessionPtr;

	class TcpSession;

	template<typename Key, class S>
	class FlowSession;

	class ISender;
	typedef std::shared_ptr<ISender>	ISenderPtr;

	class ServerBase;

	class SSLSession;
	
	class SSLServer;

	class SSLContext;

	class Socks5ClientSession;

	class Socks5ClientContext;

	class Socks5ServerSession;

	class Socks5ClientContext;

	class MessageQueue;

	class DeadlineTimer;

	namespace asio = boost::asio;

} }

#endif 