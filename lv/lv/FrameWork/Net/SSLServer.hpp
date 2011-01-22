// *********************************************************************
//  SSLServer   version:  1.0   ¡¤  date: 04/25/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SSLSERVER_HPP
#define LV_NET_SSLSERVER_HPP

#include <lv/FrameWork/Net/ServerBase.hpp>
#include <lv/FrameWork/Net/SSLContext.hpp>
#include <lv/SharedPtr.hpp>

#include <boost/asio/ssl.hpp>

namespace lv { namespace net {


	template<class S>
	class SSLServer : public ServerBase<S>
	{
		typedef ServerBase<S>	base_type;

	protected:

		asio::ssl::context	ssl_context_;
		
		std::string	password_;

	public:

		SSLServer(ContextPtr context, std::string const & password = "rswvfbuk")
			: base_type(context)
			, password_(password)
			, ssl_context_(context->service(), asio::ssl::context::sslv23)
		{
			boost::dynamic_pointer_cast<SSLContext>(context_)->set_ssl_context(lv::shared_from_object(ssl_context_));
		}

		/// @exception boost::system::system_error
		virtual	void	start(unsigned short port)
		{
			init_context();

			base_type::start(port);
		}


	protected:

		virtual	void	init_context()
		{
			ssl_context_.set_options(
				asio::ssl::context::default_workarounds
				| asio::ssl::context::no_sslv2
				| asio::ssl::context::single_dh_use);

			ssl_context_.set_password_callback(boost::bind(&SSLServer::get_password, this));
			ssl_context_.use_certificate_file("cert/rscert.pem", asio::ssl::context::pem);
			ssl_context_.use_private_key_file("cert/rsprivkey.pem", asio::ssl::context::pem);
			ssl_context_.use_tmp_dh_file("cert/dh256.pem");

		}

		virtual	std::string	get_password()
		{
			return password_;
		}
	};

} }

#endif