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

namespace boost { namespace asio {} }

namespace lv { namespace net {

	class Context;

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

	
	namespace asio = boost::asio;

} }

#endif 