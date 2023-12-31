#include <lv/FrameWork/Net/SSLSession.hpp>
#include <lv/FrameWork/Net/SSLContext.hpp>
#include <lv/FrameWork/Net/SocketHolder.hpp>

#include <boost/asio/ssl.hpp>
#include <boost/asio/placeholders.hpp>


namespace lv::net
{
	class SSLSocketHolder : public SocketHolder
	{
	public:

		using socket_type = asio::ssl::stream<asio::ip::tcp::socket>;

	private:

		socket_type	socket_;

	public:

		SSLSocketHolder(ContextPtr context)
			: socket_(context->io_context(), *dynamic_cast<SSLContext &>(*context).ssl_context())
		{
		}

		asio::ip::tcp::socket & get() override
		{
			return socket_.next_layer();
		}

		socket_type &	real_socket()
		{
			return socket_;
		}
	};


	SSLSession::SSLSession(ContextPtr context)
		: base_type(context, std::make_shared<SSLSocketHolder>(context))
	{
	}

	//
	void SSLSession::server_side_start()
	{
		handshake(HandshakeType::Server);
	}

	void SSLSession::handle_handshake(boost::system::error_code const & error)
	{
		if (closed())
		{
			return;
		}

		if (!error)
		{
			on_connected_internal();
		}
		else
		{
			on_error_internal(ErrorHandshake, error);
		}
	}

	void SSLSession::handle_connect(boost::system::error_code const & error)
	{
		if (closed())
		{
			return;
		}

		if (!error)
		{
			handshake(HandshakeType::Client);
		}
		else
		{
			on_error_internal(ErrorConnect, error);
		}
	}

	void SSLSession::handshake(HandshakeType type)
	{
		asio::ssl::stream_base::handshake_type true_type{};

		switch (type)
		{
		case HandshakeType::Client:
			true_type = asio::ssl::stream_base::client;
			break;

		case HandshakeType::Server:
			true_type = asio::ssl::stream_base::server;
			break;
		}


		SSLSocketHolder::socket_type & sock = dynamic_cast<SSLSocketHolder &>(*socket()).real_socket();

		sock.async_handshake(true_type, std::bind(&SSLSession::handle_handshake, 
			std::dynamic_pointer_cast<SSLSession>(shared_from_this()), std::placeholders::_1));
	}

}
