// *********************************************************************
//  SSLSession   version:  1.0   ¡¤  date: 04/25/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SSLSESSION_HPP
#define LV_NET_SSLSESSION_HPP

#include <lv/FrameWork/Net/CsSession.hpp>
#include <lv/FrameWork/Net/SSLContext.hpp>

#include <boost/asio/ssl.hpp>

namespace lv { namespace net {

	template<typename Side>
	class SSLSession : public CsSession<Side>
	{
		typedef CsSession<Side>	base_type;

		typedef asio::ssl::stream<asio::ip::tcp::socket> ssl_socket;

		ssl_socket	socket_;

	public:

		explicit SSLSession(ContextPtr context)
			: base_type(context)
			, socket_(context->service(), 
				*boost::dynamic_pointer_cast<SSLContext>(context)->get_ssl_context())
		{
		}

		virtual	asio::ip::tcp::socket & socket()
		{
			return socket_.next_layer();
		}

	protected:

		virtual	void	on_connected_internal()
		{
			asio::ssl::stream_base::handshake_type type;
			if(boost::is_same<Side, ClientSide>::value)
			{
				type = asio::ssl::stream_base::client;
			}
			else
			{
				type = asio::ssl::stream_base::server;
			}

			socket_.async_handshake(type, boost::bind(&SSLSession::handle_handshake, 
				boost::shared_static_cast<SSLSession>(shared_from_this()), asio::placeholders::error));
		}

		virtual void	handle_handshake(boost::system::error_code const & error)
		{
			if(! error)
			{
				base_type::on_connected_internal();
			}
			else
			{
				on_error(ErrorHandshake, error);
			}
		}

	};


} }

#endif