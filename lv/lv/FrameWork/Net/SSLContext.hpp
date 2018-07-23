// *********************************************************************
//  SSLContext   version:  1.0   ¡¤  date: 05/11/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/Context.hpp>

#include <boost/asio/ssl/context.hpp>


namespace lv::net
{
	class SSLContext : public Context
	{
		using ssl_context_ptr = std::shared_ptr<asio::ssl::context>;
		ssl_context_ptr	ssl_context_;

	public:

		SSLContext(IOContextWrapper const & io_wrapper, asio::ssl::context::method method = asio::ssl::context::sslv23)
			: Context(io_wrapper)
			, ssl_context_(std::make_shared<asio::ssl::context>(method))
		{
		}

		ssl_context_ptr	ssl_context() const
		{
			return ssl_context_;
		}
	};

}

