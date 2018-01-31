// *********************************************************************
//  SocketHolder   version:  1.0   ¡¤  date: 2013/01/23
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/Fwd.hpp>

#include <boost/asio/ip/tcp.hpp>


namespace lv::net
{
	class SocketHolder
	{
	public:

		virtual asio::ip::tcp::socket & get() = 0;
	};

}
