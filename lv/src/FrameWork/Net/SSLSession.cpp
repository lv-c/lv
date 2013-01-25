#include <lv/FrameWork/Net/SSLSession.hpp>
#include <lv/FrameWork/Net/SSLContext.hpp>
#include <lv/FrameWork/Net/SocketHolder.hpp>

#include <boost/asio/ssl.hpp>


namespace lv { namespace net {


	class SSLSocketHolder : public SocketHolder
	{
	public:

		typedef asio::ssl::stream<asio::ip::tcp::socket> socket_type;

	private:

		socket_type	socket_;

	public:

		SSLSocketHolder(ContextPtr context)
			: socket_(context->service(), *boost::dynamic_pointer_cast<SSLContext>(context)->get_ssl_context())
		{
		}

		virtual asio::ip::tcp::socket & get()
		{
			return socket_.next_layer();
		}

		socket_type &	real_socket()
		{
			return socket_;
		}
	};


	SSLSession::SSLSession(ContextPtr context)
		: base_type(context, SocketHolderPtr(new SSLSocketHolder(context)))
	{
	}

	//
	void SSLSession::server_side_start()
	{
		handshake(ServerHandshake);
	}


	void SSLSession::on_connected_internal()
	{
	}

	void SSLSession::handle_handshake(boost::system::error_code const & error)
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

	void SSLSession::handle_connect(boost::system::error_code const & error)
	{
		if(! error)
		{
			handshake(ClientHandshake);
		}
		else
		{
			base_type::handle_connect(error);
		}
	}

	void SSLSession::handshake(HandshakeType type)
	{
		asio::ssl::stream_base::handshake_type true_type;

		switch(type)
		{
		case ClientHandshake:
			true_type = asio::ssl::stream_base::client;
			break;

		case ServerHandshake:
			true_type = asio::ssl::stream_base::server;
			break;

		default:
			BOOST_ASSERT(false);
		}


		SSLSocketHolder::socket_type & sock = boost::shared_dynamic_cast<SSLSocketHolder>(socket())->real_socket();

		sock.async_handshake(true_type, boost::bind(&SSLSession::handle_handshake, 
			boost::shared_static_cast<SSLSession>(shared_from_this()), asio::placeholders::error));
	}

} }