#include <lv/FrameWork/Net/Socks5Session.hpp>
#include <lv/FrameWork/Net/Socks5Context.hpp>
#include <lv/BinaryStream/BinaryIStream.hpp>
#include <lv/BinaryStream/Manipulators.hpp>
#include <lv/BinaryStream/Array.hpp>

#include <lv/Stream/IBufferStream.hpp>

#include <boost/bind.hpp>

namespace lv { namespace net {

	Socks5Session::Socks5Session(ContextPtr context)
		: base_type(context)
		, use_proxy_(false)
		, status_(None)
		, first_error_(true)
	{
		BOOST_ASSERT(boost::shared_dynamic_cast<Socks5Context>(context));
	}

	void Socks5Session::start(std::string const & ip, std::string const & port)
	{
		first_error_ = true;
		status_ = None;
		cache_.reset();

		std::string socks_ip, socks_port;
		boost::shared_dynamic_cast<Socks5Context>(context_)->get_proxy(socks_ip, socks_port);

		use_proxy_ = (! socks_ip.empty());

		if(use_proxy_)
		{
			boost::asio::ip::tcp::resolver resolver(context_->service());
			boost::asio::ip::tcp::resolver::query query(ip, port);
			endpoint_ = *resolver.resolve(query);

			base_type::start(socks_ip, socks_port);
		}
		else
			base_type::start(ip, port);
	}

	void Socks5Session::handle_connect(boost::system::error_code const & error)
	{
		if(! use_proxy_)
			base_type::handle_connect(error);
		else
		{
			if(error)
				on_error_internal(ErrorHandshake, error);
			else
				on_connected_internal();
		}
	}

	void Socks5Session::on_connected_internal()
	{
		if(! use_proxy_)
		{
			status_ = Established;
			base_type::on_connected_internal();
		}
		else
		{
			bool auth = boost::shared_dynamic_cast<Socks5Context>(context_)->has_auth();
			uint8 const methods_num = 1;

			status_ = MethodSelect;
			socks_send() << version_ << methods_num << uint8(auth ? UserPassword : NoAuth);

			start_read();
		}
	}

	void Socks5Session::handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error)
	{
		if(status_ == Established)
		{
			base_type::handle_read(buf, bytes_transferred, error);
			return;
		}

		if(error)
		{
			on_error_internal(ErrorHandshake, error);
			return;
		}

		if(status_ == None)
		{
			BOOST_ASSERT(false);
			return;
		}

		if(cache_)
		{
			cache_->insert(cache_->end(), buf->begin(), buf->end());
			buf = cache_;
		}


		IBufferStream raw_is(buf);
		BinaryIStream bis(raw_is);

		try
		{
			if(status_ == MethodSelect)
			{
				uint8 ver, method;
				bis >> ver >> method;
				if(ver != version_ || (method != NoAuth && method != UserPassword))
				{
					on_error_internal(ErrorHandshake, boost::asio::error::make_error_code(boost::asio::error::operation_not_supported));
					close();
					return;
				}
		
				if(method == UserPassword)
				{
					boost::shared_ptr<Socks5Context> s5_context = boost::shared_dynamic_cast<Socks5Context>(context_);
					std::string user, password;
					s5_context->get_auth(user, password);

					status_ = Auth;
					socks_send() << version_ << bstream::variable_len_str<uint8>(user)
						<< bstream::variable_len_str<uint8>(password);
				}
				else
					send_request();
			}
			else if(status_ == Auth)
			{
				uint8 ver, ret;
				bis >> ver >> ret;
				if(ret != 0)
				{
					on_error_internal(ErrorHandshake, boost::asio::error::make_error_code(boost::asio::error::no_permission));
					close();
					return;
				}

				send_request();
			}
			else if(status_ == Request)
			{
				uint8 ver, rep, addr_type;
				bis >> ver >> rep >> bstream::forward(1) >> addr_type;

				// we don't need the following data but we should still read them all
				switch(addr_type)
				{
				case IPV4:
					bis >> bstream::forward(4);
					break;
				case DomainName:
					{
						std::string tmp;
						bis >> bstream::variable_len_str<uint8>(tmp);
						break;
					}
				case IPV6:
					bis >> bstream::forward(16);
					break;
				default:
					BOOST_ASSERT(false);
				}

				bis >> bstream::forward(2);		// port

				//
				if(rep == 0)
				{
					status_ = Established;
					base_type::on_connected_internal();
				}
				else
				{
					ErrorType err_type;
					boost::system::error_code err;
					gen_request_error(rep, err_type, err);

					on_error_internal(err_type, err);
					close();
				}

				return;
			}
		}
		catch (std::ios_base::failure const &)
		{
			// an incomplete packet
			cache_ = buf;
		}

		start_read();
	}

	void Socks5Session::gen_request_error(uint8 rep, ErrorType & err_type, boost::system::error_code & error)
	{
		boost::asio::error::basic_errors asio_err;

		switch(rep)
		{
		case 1:		// general SOCKS server failure
			asio_err = boost::asio::error::connection_refused;	// .....
			err_type = ErrorConnect;
			break;

		case 2:		// connection not allowed by ruleset
			asio_err = boost::asio::error::access_denied;
			err_type = ErrorHandshake;
			break;

		case 3:		// Network unreachable
			asio_err = boost::asio::error::network_unreachable;
			err_type = ErrorConnect;
			break;

		case 4:		// Host unreachable
			asio_err = boost::asio::error::host_unreachable;
			err_type = ErrorConnect;
			break;

		case 5:		// Connection refused
			asio_err = boost::asio::error::connection_refused;
			err_type = ErrorConnect;
			break;

		case 6:		// TTL expired
			asio_err = boost::asio::error::timed_out;
			err_type = ErrorConnect;
			break;

		case 7:		// Command not supported
			asio_err = boost::asio::error::operation_not_supported;
			err_type = ErrorHandshake;
			break;

		case 8:		// Address type not supported
			asio_err = boost::asio::error::address_family_not_supported;
			err_type = ErrorHandshake;
			break;

		default:
			asio_err = boost::asio::error::connection_refused;	// .....
			err_type = ErrorHandshake;
		}

		error = boost::asio::error::make_error_code(asio_err);
	}

	void Socks5Session::handle_write(BufferPtr buf, boost::system::error_code const & error)
	{
		if(status_ == Established)
			base_type::handle_write(buf, error);
		else if(error)
			on_error_internal(ErrorHandshake, error);
	}

	void Socks5Session::send_request()
	{
		status_ = Request;

		uint8 const cmd = 1;	// CONNECT
		PacketProxy proxy = socks_send() << version_ << cmd << uint8(0);

		if(endpoint_.protocol() == boost::asio::ip::tcp::v4())
		{
			proxy << IPV4 << endpoint_.address().to_v4().to_bytes();
		}
		else
		{
			BOOST_ASSERT(endpoint_.protocol() == boost::asio::ip::tcp::v6());
			proxy << IPV6 << endpoint_.address().to_v6().to_bytes();
		}

		unsigned short port = endpoint_.port();
		uint8 high_byte = (port >> 8) & 0xFF;
		uint8 low_byte = port & 0xFF;

		proxy << high_byte << low_byte;
	}

	PacketProxy Socks5Session::socks_send()
	{
		return PacketProxy(context_->buffer(), boost::bind(&Socks5Session::start_write, this, _1));
	}

	void Socks5Session::on_error_internal(ErrorType type, boost::system::error_code const & error)
	{
		// TODO : not thread-safe?
		if(first_error_)
		{
			first_error_ = false;
			on_error(type, error);
		}
	}

} }