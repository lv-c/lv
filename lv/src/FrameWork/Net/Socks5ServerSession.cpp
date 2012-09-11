#include <lv/FrameWork/Net/Socks5ServerSession.hpp>
#include <lv/FrameWork/Net/Socks5ServerContext.hpp>
#include <lv/FrameWork/Net/Socks5.hpp>
#include <lv/BinaryStream/BinaryIStream.hpp>
#include <lv/BinaryStream/Manipulators.hpp>
#include <lv/BinaryStream/String.hpp>
#include <lv/BinaryStream/Vector.hpp>
#include <lv/BinaryStream/Array.hpp>
#include <lv/Stream/IBufferStream.hpp>

#include <boost/lexical_cast.hpp>

namespace lv { namespace net {

	Socks5ServerSession::Socks5ServerSession(ContextPtr context)
		: base_type(context)
		, status_(MethodSelect)
		, closed_(false)
	{
		BOOST_ASSERT(boost::shared_dynamic_cast<ISocks5ServerContext>(context));
	}

	void Socks5ServerSession::on_receive(BufferPtr buf)
	{
		if(status_ == Established)
		{
			dest_session_->start_write(buf);
			return;
		}
		else if(status_ == Ended)
		{
			return;
		}

		if(cache_)
		{
			cache_->insert(cache_->end(), buf->begin(), buf->end());
			buf = cache_;
		}

		BinaryIStream bis(buf);

		try
		{
			switch(status_)
			{
			case MethodSelect:
				handle_method_select(bis);
				break;

			case Auth:
				handle_auth(bis);
				break;

			case Request:
				handle_request(bis);
				break;
			}
		}
		catch (std::ios_base::failure const &)
		{
			// an incomplete packet
			cache_ = buf;
		}
	}

	void Socks5ServerSession::on_write(BufferPtr buf)
	{
		//if(status_ == Ended)
		//	exit();
	}

	void Socks5ServerSession::handle_method_select(BinaryIStream & bis)
	{
		uint8 ver, methods_num;
		bis >> ver >> methods_num;
	
		if(ver != Socks5::Version || methods_num == 0)
		{
			exit();
			return;
		}

		std::vector<uint8> methods(methods_num);
		bis >> methods;

		uint8 m = boost::shared_dynamic_cast<ISocks5ServerContext>(context_)->select_method(methods);

		send() << Socks5::Version << m;

		switch(m)
		{
		case Socks5::NoAuth:
			status_ = Request;
			break;

		case Socks5::UserPassword:
			status_ = Auth;
			break;

		case Socks5::NoAcceptable:
			exit();
			break;

		default:
			BOOST_ASSERT(false);
			exit();
		}
	}

	void Socks5ServerSession::handle_auth(BinaryIStream & bis)
	{
		uint8 ver;
		Socks5Auth auth;

		bis >> ver >> bstream::variable_len_range<uint8>(auth.user)
			>> bstream::variable_len_range<uint8>(auth.password);

		BOOST_ASSERT(ver == Socks5::AuthVersion);

		bool valid = boost::shared_dynamic_cast<ISocks5ServerContext>(context_)->verify(auth);

		send() << Socks5::AuthVersion << uint8(valid ? 0 : 1);

		if(valid)
		{
			status_ = Request;
		}
		else
		{
			exit();
		}
	}

	void Socks5ServerSession::handle_request(BinaryIStream & bis)
	{
		uint8 ver, cmd, addr_type;

		bis >> ver >> cmd >> bstream::forward(1) >> addr_type;

		std::string host;

		boost::system::error_code error;

		switch(addr_type)
		{
		case Socks5::IPV4:
			{
				boost::asio::ip::address_v4::bytes_type bytes;
				bis >> bytes;
				host = boost::asio::ip::address_v4(bytes).to_string(error);
				break;
			}

		case Socks5::IPV6:
			{
				boost::asio::ip::address_v6::bytes_type bytes;
				bis >> bytes;
				host = boost::asio::ip::address_v6(bytes).to_string(error);
				break;
			}

		case Socks5::DomainName:
			bis >> bstream::variable_len_range<uint8>(host);
			break;

		default:
			error = boost::asio::error::make_error_code(boost::asio::error::operation_not_supported);
		}

		if(cmd != 1)	// CONNECT
		{
			error = boost::asio::error::make_error_code(boost::asio::error::operation_not_supported);
		}

		if(error)
		{
			send_request_response(error);
			return;
		}

		uint8 high_byte, low_byte;
		bis >> high_byte >> low_byte;

		uint16 port = (high_byte << 8) | low_byte;

		ContextPtr context(new Context(context_->buffer_manager(), context_->service_wrapper()));

		dest_session_.reset(new DestSession(context));

		BOOST_ASSERT(connections_.empty());

		connections_.push_back(dest_session_->connect_event().connect(boost::bind(&Socks5ServerSession::dest_on_connected, this)));
		connections_.push_back(dest_session_->error_event().connect(boost::bind(&Socks5ServerSession::dest_on_error, this, _1, _2)));
		connections_.push_back(dest_session_->receive_event().connect(boost::bind(&Socks5ServerSession::dest_on_receive, this, _1)));

		boost::asio::ip::address to_bind = boost::shared_dynamic_cast<ISocks5ServerContext>(context_)->address_to_bind();

		try
		{
			dest_session_->start(host, boost::lexical_cast<std::string>(port), to_bind);
		}
		catch (boost::system::system_error const & err)
		{
			send_request_response(err.code());
			return;
		}
	}

	void Socks5ServerSession::send_request_response(boost::system::error_code error)
	{
		boost::asio::ip::address addr;
		unsigned short port = 0;

		if(! error)
		{
			boost::asio::ip::tcp::endpoint endpoint = dest_session_->socket().local_endpoint(error);
			if(! error)
			{
				addr = endpoint.address();
				port = endpoint.port();
			}
		}

		uint8 rep = error_to_rep(error);

		PacketProxy proxy = send() << Socks5::Version << rep << uint8(0);

		if(! error)
		{
			uint8 high_byte = (port >> 8) & 0xFF;
			uint8 low_byte = port & 0xFF;

			if(addr.is_v4())
			{
				proxy << Socks5::IPV4 << addr.to_v4().to_bytes();
			}
			else
			{
				BOOST_ASSERT(addr.is_v6());
				proxy << Socks5::IPV6 << addr.to_v6().to_bytes();
			}

			proxy << high_byte << low_byte;

			status_ = Established;
		}
		else
		{
			proxy << Socks5::IPV4 << boost::asio::ip::address_v4::bytes_type() << uint16(0);

			exit();
		}
	}

	uint8 Socks5ServerSession::error_to_rep(boost::system::error_code const & error)
	{
		if(! error)
		{
			return 0;
		}

		if(error.category() == boost::asio::error::get_system_category())
		{
			switch(error.value())
			{
			case boost::asio::error::network_unreachable:
				return 3;		// Network unreachable
				
			case boost::asio::error::host_unreachable:
				return 4;		// Host unreachable

			case boost::asio::error::connection_refused:
				return 5;		// Connection refused

			case boost::asio::error::timed_out:
				return 6;		// TTL expired

			case boost::asio::error::operation_not_supported:
				return 7;		// Command not supported

			case boost::asio::error::address_family_not_supported:
				return 8;		// Address type not supported

			}
		}

		return 1;		// general SOCKS server failure
	}

	void Socks5ServerSession::dest_on_connected()
	{
		send_request_response(boost::system::error_code());
	}

	void Socks5ServerSession::dest_on_error(ErrorType type, boost::system::error_code const & error)
	{
		if(status_ == Ended)
		{
			return;
		}

		if(type == ErrorConnect)
		{
			send_request_response(error);
		}
		else
		{
			exit();
		}
	}

	void Socks5ServerSession::dest_on_receive(BufferPtr buf)
	{
		start_write(buf);
	}

	void Socks5ServerSession::on_error(ErrorType type, boost::system::error_code const & error)
	{
		exit();

		if(! closed_)
		{
			closed_ = true;
			close();
		}
	}

	void Socks5ServerSession::exit()
	{
		if(status_ == Ended)
		{
			return;
		}

		status_ = Ended;

		if(dest_session_)
		{
			foreach(boost::signals2::connection & conn, connections_)
			{
				conn.disconnect();
			}
			connections_.clear();

			dest_session_->close();
			dest_session_.reset();
		}

		shutdown();
	}

	PacketProxy Socks5ServerSession::send()
	{
		return PacketProxy(context_->buffer(), boost::bind(&Socks5ServerSession::start_write, this, _1));
	}

} }