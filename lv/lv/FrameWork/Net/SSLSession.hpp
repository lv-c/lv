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

#include <lv/FrameWork/Net/SessionBase.hpp>
#include <lv/FrameWork/Net/SSLContext.hpp>

#include <boost/asio/ssl.hpp>

namespace lv { namespace net {

	class SSLSession : public SessionBase
	{
		typedef SessionBase	base_type;

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

		//
		virtual	void	server_side_start()
		{
			handshake(asio::ssl::stream_base::server);
		}

	protected:

		virtual	void	on_connected_internal()
		{
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

		virtual	void	handle_connect(boost::system::error_code const & error)
		{
			if(! error)
			{
				handshake(asio::ssl::stream_base::client);
			}
			else
			{
				base_type::handle_connect(error);
			}
		}

	private:

		void	handshake(asio::ssl::stream_base::handshake_type type)
		{
			socket_.async_handshake(type, boost::bind(&SSLSession::handle_handshake, 
				boost::shared_static_cast<SSLSession>(shared_from_this()), asio::placeholders::error));
		}

	};


} }

#endif