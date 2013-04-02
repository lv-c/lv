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

#include <boost/shared_ptr.hpp>

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
	typedef boost::shared_ptr<Context>	ContextPtr;

	class SocketHolder;
	typedef boost::shared_ptr<SocketHolder>	SocketHolderPtr;

	class SessionBase;
	typedef boost::shared_ptr<SessionBase>	SessionPtr;

	class TcpSession;

	template<typename Key, class S>
	class FlowSession;

	class ServerBase;

	class SSLSession;
	
	class SSLServer;

	class SSLContext;

	namespace asio = boost::asio;

} }

#endif 