// *********************************************************************
//  SocketHolder   version:  1.0   ¡¤  date: 2013/01/23
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SOCKETHOLDER_HPP
#define LV_NET_SOCKETHOLDER_HPP

#include <lv/FrameWork/Net/Fwd.hpp>

#include <boost/asio/ip/tcp.hpp>

namespace lv { namespace net {

	class SocketHolder
	{
	public:

		virtual asio::ip::tcp::socket & get() = 0;
	};

} }


#endif