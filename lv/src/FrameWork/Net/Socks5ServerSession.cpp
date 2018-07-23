#include <lv/FrameWork/Net/Socks5ServerSession.hpp>
#include <lv/FrameWork/Net/Socks5ServerContext.hpp>
#include <lv/FrameWork/Net/Socks5.hpp>
#include <lv/FrameWork/Net/SocketHolder.hpp>
#include <lv/BinaryStream/BinaryIStream.hpp>
#include <lv/BinaryStream/Manipulators.hpp>
#include <lv/BinaryStream/String.hpp>
#include <lv/BinaryStream/Vector.hpp>
#include <lv/BinaryStream/Array.hpp>
#include <lv/Stream/IBufferStream.hpp>

#include <boost/asio/ip/tcp.hpp>


namespace lv::net
{
	Socks5ServerSession::Socks5ServerSession(ContextPtr context)
		: base_type(context)
		, status_(MethodSelect)
		, closed_(false)
	{
		BOOST_ASSERT(std::dynamic_pointer_cast<ISocks5ServerContext>(context));
	}

	void Socks5ServerSession::on_receive(Buffer const & buf)
	{
		if (status_ == Established)
		{
			dest_session_->start_write(buf);
			return;
		}
		else if (status_ == Ended)
		{
			return;
		}

		Buffer temp_buf = std::move(cache_);
		buffer::append(temp_buf, buf);

		BinaryIStream bis(temp_buf);

		try
		{
			switch (status_)
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

			default:
				BOOST_ASSERT(false);
				break;
			}
		}
		catch (std::ios_base::failure const &)
		{
			// an incomplete packet
			cache_ = std::move(temp_buf);
		}
	}

	void Socks5ServerSession::on_write(size_t size)
	{
		//if (status_ == Ended)
		//	exit();
	}

	void Socks5ServerSession::handle_method_select(BinaryIStream & bis)
	{
		uint8_t ver, methods_num;
		bis >> ver >> methods_num;
	
		if (ver != Socks5::Version || methods_num == 0)
		{
			exit();
			return;
		}

		std::vector<uint8_t> methods(methods_num);
		bis >> methods;

		uint8_t m = dynamic_cast<ISocks5ServerContext &>(*context_).select_method(methods);

		send() << Socks5::Version << m;

		switch (m)
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
		uint8_t ver;
		Socks5Auth auth;

		bis >> ver >> bstream::variable_len_range<uint8_t>(auth.user)
			>> bstream::variable_len_range<uint8_t>(auth.password);

		BOOST_ASSERT(ver == Socks5::AuthVersion);

		bool valid = dynamic_cast<ISocks5ServerContext &>(*context_).verify(auth);

		send() << Socks5::AuthVersion << uint8_t(valid ? 0 : 1);

		if (valid)
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
		uint8_t ver, cmd, addr_type;

		bis >> ver >> cmd >> bstream::forward(1) >> addr_type;

		std::string host;

		boost::system::error_code error;

		switch (addr_type)
		{
		case Socks5::IPV4:
			{
				asio::ip::address_v4::bytes_type bytes;
				bis >> bytes;
				host = asio::ip::address_v4(bytes).to_string(error);
				break;
			}

		case Socks5::IPV6:
			{
				asio::ip::address_v6::bytes_type bytes;
				bis >> bytes;
				host = asio::ip::address_v6(bytes).to_string(error);
				break;
			}

		case Socks5::DomainName:
			bis >> bstream::variable_len_range<uint8_t>(host);
			break;

		default:
			error = asio::error::make_error_code(asio::error::operation_not_supported);
		}

		if (cmd != 1)	// CONNECT
		{
			error = asio::error::make_error_code(asio::error::operation_not_supported);
		}

		if (error)
		{
			send_request_response(error);
			return;
		}

		uint8_t high_byte, low_byte;
		bis >> high_byte >> low_byte;

		uint16_t port = (high_byte << 8) | low_byte;

		ContextPtr context = std::make_shared<Context>(context_->io_wrapper());

		dest_session_ = std::make_unique<DestSession>(context);

		BOOST_ASSERT(connections_.empty());

		connections_.push_back(dest_session_->connect_event().connect(std::bind(&Socks5ServerSession::dest_on_connected, this)));
		connections_.push_back(dest_session_->error_event().connect(std::bind(&Socks5ServerSession::dest_on_error, this, std::placeholders::_1, std::placeholders::_2)));
		connections_.push_back(dest_session_->receive_event().connect(std::bind(&Socks5ServerSession::dest_on_receive, this, std::placeholders::_1)));

		std::string to_bind = dynamic_cast<ISocks5ServerContext &>(*context_).address_to_bind();

		try
		{
			dest_session_->connect(host, std::to_string(port), to_bind);
		}
		catch (boost::system::system_error const & err)
		{
			send_request_response(err.code());
			return;
		}
	}

	void Socks5ServerSession::send_request_response(boost::system::error_code error)
	{
		asio::ip::address addr;
		unsigned short port = 0;

		if (!error)
		{
			asio::ip::tcp::endpoint endpoint = dest_session_->socket()->get().local_endpoint(error);
			if (!error)
			{
				addr = endpoint.address();
				port = endpoint.port();
			}
		}

		uint8_t rep = error_to_rep(error);

		PacketProxy proxy = std::move(send() << Socks5::Version << rep << uint8_t(0));

		if (!error)
		{
			uint8_t high_byte = (port >> 8) & 0xFF;
			uint8_t low_byte = port & 0xFF;

			if (addr.is_v4())
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
			proxy << Socks5::IPV4 << asio::ip::address_v4::bytes_type() << uint16_t(0);

			exit();
		}
	}

	uint8_t Socks5ServerSession::error_to_rep(boost::system::error_code const & error)
	{
		if (!error)
		{
			return 0;
		}

		if (error.category() == asio::error::get_system_category())
		{
			switch (error.value())
			{
			case asio::error::network_unreachable:
				return 3;		// Network unreachable
				
			case asio::error::host_unreachable:
				return 4;		// Host unreachable

			case asio::error::connection_refused:
				return 5;		// Connection refused

			case asio::error::timed_out:
				return 6;		// TTL expired

			case asio::error::operation_not_supported:
				return 7;		// Command not supported

			case asio::error::address_family_not_supported:
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
		if (status_ == Ended)
		{
			return;
		}

		if (type == ErrorConnect)
		{
			send_request_response(error);
		}
		else
		{
			exit();
		}
	}

	void Socks5ServerSession::dest_on_receive(Buffer const & buf)
	{
		start_write(buf);
	}

	void Socks5ServerSession::on_error(ErrorType type, boost::system::error_code const & error)
	{
		exit();

		if (!closed_)
		{
			closed_ = true;
			close();
		}
	}

	void Socks5ServerSession::exit()
	{
		if (status_ == Ended)
		{
			return;
		}

		status_ = Ended;

		if (dest_session_)
		{
			for (auto & conn : connections_)
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
		return PacketProxy(std::make_shared<Buffer>(1024), [this](ConstBufferRef buf) { start_write(buf); });
	}

}
