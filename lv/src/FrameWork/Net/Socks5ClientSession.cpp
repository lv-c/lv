#include <lv/FrameWork/Net/Socks5ClientSession.hpp>
#include <lv/FrameWork/Net/Socks5ClientContext.hpp>
#include <lv/FrameWork/Net/Socks5.hpp>
#include <lv/BinaryStream/BinaryIStream.hpp>
#include <lv/BinaryStream/Manipulators.hpp>
#include <lv/BinaryStream/String.hpp>
#include <lv/BinaryStream/Array.hpp>
#include <lv/Stream/IBufferStream.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <functional>


namespace lv { namespace net {

	Socks5ClientSession::Socks5ClientSession(ContextPtr context)
		: base_type(context)
		, use_proxy_(false)
		, status_(None)
	{
		BOOST_ASSERT(std::dynamic_pointer_cast<Socks5ClientContext>(context));
	}

	void Socks5ClientSession::connect(std::string const & ip, std::string const & port,
		std::string const & to_bind /* = std::string */)
	{
		status_ = None;
		cache_.reset();

		ip_ = ip;
		port_ = port;

		std::string socks_ip, socks_port;
		std::dynamic_pointer_cast<Socks5ClientContext>(context_)->get_proxy(socks_ip, socks_port);

		use_proxy_ = (! socks_ip.empty());

		if (use_proxy_)
		{
			base_type::connect(socks_ip, socks_port, to_bind);
		}
		else
		{
			base_type::connect(ip, port, to_bind);
		}
	}

	void Socks5ClientSession::handle_connect(boost::system::error_code const & error)
	{
		if (closed())
		{
			return;
		}

		if (! use_proxy_)
		{
			base_type::handle_connect(error);
		}
		else
		{
			if (error)
			{
				on_error_internal(ErrorHandshake, error);
			}
			else
			{
				on_connected_internal();
			}
		}
	}

	void Socks5ClientSession::on_connected_internal()
	{
		if (! use_proxy_)
		{
			status_ = Established;
			base_type::on_connected_internal();
		}
		else
		{
			bool auth = std::dynamic_pointer_cast<Socks5ClientContext>(context_)->has_auth();
			uint8 const methods_num = 1;

			status_ = MethodSelect;
			socks_send() << Socks5::Version << methods_num << uint8(auth ? Socks5::UserPassword : Socks5::NoAuth);

			start_read();
		}
	}

	void Socks5ClientSession::handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error)
	{
		if (closed())
		{
			return;
		}

		if (status_ == Established)
		{
			base_type::handle_read(buf, bytes_transferred, error);
			return;
		}

		if (error)
		{
			on_error_internal(ErrorHandshake, error);
			return;
		}

		if (status_ == None)
		{
			BOOST_ASSERT(false);
			return;
		}

		buf->resize(bytes_transferred);

		if (cache_)
		{
			buffer::append(*cache_, buf);
			buf = cache_;

			cache_.reset();
		}


		BinaryIStream bis(buf);

		try
		{
			switch(status_)
			{
			case MethodSelect:
				handle_method_select_response(bis);
				break;

			case Auth:
				handle_auth_response(bis);
				break;

			case Request:
				handle_request_response(bis);
				break;
			}
		}
		catch (std::ios_base::failure const &)
		{
			// an incomplete packet
			cache_ = buf;
		}

		start_read();
	}

	void Socks5ClientSession::handle_method_select_response(BinaryIStream & bis)
	{
		uint8 ver, method;
		bis >> ver >> method;
		if (ver != Socks5::Version || (method != Socks5::NoAuth && method != Socks5::UserPassword))
		{
			on_error_internal(ErrorHandshake, asio::error::make_error_code(asio::error::operation_not_supported));
			close();
			return;
		}

		if (method == Socks5::UserPassword)
		{
			std::shared_ptr<Socks5ClientContext> s5_context = std::dynamic_pointer_cast<Socks5ClientContext>(context_);
			std::string user, password;
			s5_context->get_auth(user, password);

			status_ = Auth;
			socks_send() << Socks5::AuthVersion << bstream::variable_len_range<uint8>(user)
				<< bstream::variable_len_range<uint8>(password);
		}
		else
		{
			send_request();
		}
	}

	void Socks5ClientSession::handle_auth_response(BinaryIStream & bis)
	{
		uint8 ver, ret;
		bis >> ver >> ret;
		if (ret != 0)
		{
			on_error_internal(ErrorHandshake, asio::error::make_error_code(asio::error::no_permission));
			close();
			return;
		}

		send_request();
	}

	void Socks5ClientSession::handle_request_response(BinaryIStream & bis)
	{
		uint8 ver, rep, addr_type;
		bis >> ver >> rep >> bstream::forward(1) >> addr_type;

		// we don't need the following data but we should still read them all
		switch(addr_type)
		{
		case Socks5::IPV4:
			bis >> bstream::forward(4);
			break;

		case Socks5::DomainName:
			{
				std::string tmp;
				bis >> bstream::variable_len_range<uint8>(tmp);
				break;
			}

		case Socks5::IPV6:
			bis >> bstream::forward(16);
			break;

		default:
			BOOST_ASSERT(false);
		}

		bis >> bstream::forward(2);		// port

		//
		if (rep == 0)
		{
			status_ = Established;
			on_connected();
		}
		else
		{
			ErrorType err_type;
			boost::system::error_code err;
			gen_request_error(rep, err_type, err);

			on_error_internal(err_type, err);
			close();
		}
	}

	void Socks5ClientSession::gen_request_error(uint8 rep, ErrorType & err_type, boost::system::error_code & error)
	{
		asio::error::basic_errors asio_err;

		switch(rep)
		{
		case 1:		// general SOCKS server failure
			asio_err = asio::error::connection_refused;	// .....
			err_type = ErrorConnect;
			break;

		case 2:		// connection not allowed by ruleset
			asio_err = asio::error::access_denied;
			err_type = ErrorHandshake;
			break;

		case 3:		// Network unreachable
			asio_err = asio::error::network_unreachable;
			err_type = ErrorConnect;
			break;

		case 4:		// Host unreachable
			asio_err = asio::error::host_unreachable;
			err_type = ErrorConnect;
			break;

		case 5:		// Connection refused
			asio_err = asio::error::connection_refused;
			err_type = ErrorConnect;
			break;

		case 6:		// TTL expired
			asio_err = asio::error::timed_out;
			err_type = ErrorConnect;
			break;

		case 7:		// Command not supported
			asio_err = asio::error::operation_not_supported;
			err_type = ErrorHandshake;
			break;

		case 8:		// Address type not supported
			asio_err = asio::error::address_family_not_supported;
			err_type = ErrorHandshake;
			break;

		default:
			asio_err = asio::error::connection_refused;	// .....
			err_type = ErrorHandshake;
		}

		error = asio::error::make_error_code(asio_err);
	}

	void Socks5ClientSession::handle_write(BufferPtr buf, boost::system::error_code const & error)
	{
		if (closed())
		{
			return;
		}

		if (status_ == Established)
		{
			base_type::handle_write(buf, error);
		}
		else if (error)
		{
			on_error_internal(ErrorHandshake, error);
		}
	}

	void Socks5ClientSession::send_request()
	{
		status_ = Request;

		asio::ip::tcp::resolver resolver(context_->service());
		asio::ip::tcp::resolver::query query(ip_, port_);

		boost::system::error_code err;
		asio::ip::tcp::resolver::iterator it = resolver.resolve(query, err);

		if (err && err.value() != asio::error::host_not_found)
		{
			on_error_internal(ErrorHandshake, err);
			return;
		}

		uint8 const cmd = 1;	// CONNECT
		PacketProxy proxy = socks_send() << Socks5::Version << cmd << uint8(0);

		unsigned short port = 0;

		if (err)
		{
			port = boost::lexical_cast<unsigned short>(port_);

			proxy << Socks5::DomainName << bstream::variable_len_range<uint8>(ip_);
		}
		else
		{
			asio::ip::tcp::endpoint endpoint = *it;

			port = endpoint.port();

			if (endpoint.protocol() == asio::ip::tcp::v4())
			{
				proxy << Socks5::IPV4 << endpoint.address().to_v4().to_bytes();
			}
			else
			{
				BOOST_ASSERT(endpoint.protocol() == asio::ip::tcp::v6());
				proxy << Socks5::IPV6 << endpoint.address().to_v6().to_bytes();
			}
		}

		uint8 high_byte = (port >> 8) & 0xFF;
		uint8 low_byte = port & 0xFF;

		proxy << high_byte << low_byte;
	}

	PacketProxy Socks5ClientSession::socks_send()
	{
		return PacketProxy(context_->buffer(), std::bind(&Socks5ClientSession::start_write, this, std::placeholders::_1));
	}

} }