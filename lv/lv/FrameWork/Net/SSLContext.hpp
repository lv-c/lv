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

		SSLContext(BufferManagerPtr buf_manager, service_ptr service, 
			ssl_context_ptr ssl_contex = ssl_context_ptr())
			: Context(buf_manager, service)
			, ssl_context_(ssl_contex)
		{
		}

		void	set_ssl_context(ssl_context_ptr ssl_context)
		{
			BOOST_ASSERT(! ssl_context_);

			this->ssl_context_ = ssl_context;
		}

		ssl_context_ptr	create_ssl_context(asio::ssl::context::method method = asio::ssl::context::sslv23)
		{
			BOOST_ASSERT(! ssl_context_);

			ssl_context_.reset(new asio::ssl::context(service(), method));
			return ssl_context_;
		}

		ssl_context_ptr	get_ssl_context() const
		{
			return ssl_context_;
		}

	};

} }



#endif