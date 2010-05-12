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

	class Context;
	typedef boost::shared_ptr<Context>	ContextPtr;

	class SessionBase;

	struct ServerSide;
	struct ClientSide;

	template<typename Side>
	class CsSession;

	template<typename Side>
	class TcpSession;

	template<typename Key, class S>
	class FlowSession;

	template<class S>
	class ServerBase;

	template<typename Side>
	class SSLSession;
	
	template<class S>
	class SSLServer;

	namespace asio = boost::asio;

} }

#endif 