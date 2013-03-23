// *********************************************************************
//  SSLContext   version:  1.0   ¡¤  date: 05/11/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SSLCONTEXT_HPP
#define LV_NET_SSLCONTEXT_HPP

#include <lv/FrameWork/Net/Context.hpp>

#include <boost/asio/ssl/context.hpp>

namespace lv { namespace net {

	class SSLContext : public Context
	{
		typedef boost::shared_ptr<asio::ssl::context>	ssl_context_ptr;
		ssl_context_ptr	ssl_context_;

	public:

		SSLContext(BufferManagerPtr buf_manager, ServiceWrapper const & service_wrapper,
				asio::ssl::context::method method = asio::ssl::context::sslv23)
			: Context(buf_manager, service_wrapper)
			, ssl_context_(new asio::ssl::context(service_wrapper.service(), method))
		{
		}

		ssl_context_ptr	ssl_context() const
		{
			return ssl_context_;
		}
	};

} }



#endif