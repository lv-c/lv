// *********************************************************************
//  TcpSession   version:  1.0   ¡¤  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_TCPSESSION_HPP
#define LV_NET_TCPSESSION_HPP

#include <lv/FrameWork/Net/SessionBase.hpp>

namespace lv { namespace net {

	class TcpSession : public SessionBase
	{

		typedef SessionBase	base_type;
		
		asio::ip::tcp::socket	socket_;

	public:

		explicit TcpSession(ContextPtr context)
			: base_type(context)
			, socket_(context->service())
		{
		}

		virtual asio::ip::tcp::socket & socket()
		{
			return socket_;
		}
	};

} }

#endif