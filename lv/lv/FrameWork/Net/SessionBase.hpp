// *********************************************************************
//  SessionBase   version:  1.0   ¡¤  date: 01/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SESSIONBASE_HPP
#define LV_NET_SESSIONBASE_HPP

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/FrameWork/Net/Context.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2.hpp>


namespace lv { namespace net {

	enum ErrorType
	{
		ErrorConnect,
		ErrorHandshake,
		ErrorRead,
		ErrorWrite
	};

	inline char const *	error_string(ErrorType type)
	{
		switch(type)
		{
		case ErrorConnect:
			return "ErrorConnect";
		case ErrorHandshake:
			return "ErrorHandshake";
		case ErrorRead:
			return "ErrorRead";
		case ErrorWrite:
			return "ErrorWrite";
		default:
			BOOST_ASSERT(false);
			return "";
		}
	}

	class SessionBase : public boost::enable_shared_from_this<SessionBase>
	{
	public:

		typedef boost::signals2::signal<void(ErrorType, boost::system::error_code const &)>	ErrorEvent;
		typedef boost::signals2::signal<void()>				ConnectEvent;
		typedef boost::signals2::signal<void(BufferPtr)>	ReceiveEvent;
		typedef boost::signals2::signal<void(BufferPtr)>	WriteEvent;

	private:

		ErrorEvent		error_event_;
		ConnectEvent	connect_event_;
		ReceiveEvent	receive_event_;
		WriteEvent		write_event_;

	protected:

		ContextPtr	context_;

	public:

		explicit SessionBase(ContextPtr context)
			: context_(context)
		{
		}

		virtual	~SessionBase()
		{
		}

		std::string		remote_ip()
		{
			boost::system::error_code error;
			asio::ip::tcp::endpoint endpoint = socket().remote_endpoint(error);

			if(! error)
			{
				return endpoint.address().to_string(error);
			}

			return std::string();
		}

		ErrorEvent &	error_event()
		{
			return error_event_;
		}

		ConnectEvent &	connect_event()
		{
			return connect_event_;
		}

		ReceiveEvent &	receive_event()
		{
			return receive_event_;
		}

		WriteEvent &	write_event()
		{
			return write_event_;
		}

		virtual	asio::ip::tcp::socket & socket() = 0;

		virtual	void	shutdown()
		{
			boost::system::error_code error;
			socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
		}

		virtual	void	close()
		{
			boost::system::error_code error;
			socket().close(error);
		}

		/// @exception runtime_error
		virtual	void	connect(std::string const & ip, std::string const & port,
			boost::asio::ip::address const & to_bind = boost::asio::ip::address())
		{
			asio::ip::tcp::resolver::query query(ip, port);
			asio::ip::tcp::resolver resolver(context_->service());

			if(to_bind != boost::asio::ip::address())
			{
				socket().open(to_bind.is_v4() ? boost::asio::ip::tcp::v4() : boost::asio::ip::tcp::v6());
				socket().bind(boost::asio::ip::tcp::endpoint(to_bind, 0));
			}

			if(context_->has_strand())
			{
				socket().async_connect(*resolver.resolve(query), 
					context_->strand().wrap(boost::bind(&SessionBase::handle_connect, shared_from_this(), asio::placeholders::error)));
			}
			else
			{
				socket().async_connect(*resolver.resolve(query), 
					boost::bind(&SessionBase::handle_connect, shared_from_this(), asio::placeholders::error));
			}
		}

		virtual void	start_read()
		{
			BufferPtr buf = context_->buffer();

			buf->resize(buf->capacity());

			if(context_->has_strand())
			{
				socket().async_read_some(asio::buffer(*buf), context_->strand().wrap(
					boost::bind(&SessionBase::handle_read, shared_from_this(), buf,
					asio::placeholders::bytes_transferred, asio::placeholders::error)));
			}
			else
			{
				socket().async_read_some(asio::buffer(*buf), 
					boost::bind(&SessionBase::handle_read, shared_from_this(), buf,
					asio::placeholders::bytes_transferred, asio::placeholders::error));
			}
		}

		virtual	void	start_write(BufferPtr buf)
		{
			if(context_->has_strand())
			{
				boost::asio::async_write(socket(), asio::buffer(*buf), context_->strand().wrap(
					boost::bind(&SessionBase::handle_write, shared_from_this(), 
					buf, asio::placeholders::error)));
			}
			else
			{
				boost::asio::async_write(socket(), asio::buffer(*buf), 
					boost::bind(&SessionBase::handle_write, shared_from_this(), 
					buf, asio::placeholders::error));
			}
		}

		// For internal use. You should use on_connected instead.
		virtual	void	server_side_start()
		{
			on_connected_internal();
		}

	protected:

		virtual	void	on_error(ErrorType type, boost::system::error_code const & error)
		{
			error_event_(type, error);
		}

		virtual	void	on_connected_internal()
		{
			start_read();
			on_connected();
		}

		virtual	void	on_connected()
		{
			connect_event_();
		}

		virtual	void	on_receive(BufferPtr buf)
		{
			receive_event_(buf);
		}

		virtual	void	on_write(BufferPtr buf)
		{
			write_event_(buf);
		}

		virtual	void	handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error)
		{
			if(error)
			{
				on_error(ErrorRead, error);
			}
			else
			{
				buf->resize(bytes_transferred);
				on_receive(buf);

				start_read();
			}
		}

		virtual	void	handle_write(BufferPtr buf, boost::system::error_code const & error)
		{
			if(error)
			{
				on_error(ErrorWrite, error);
			}
			else
			{
				on_write(buf);
			}
		}

		virtual	void	handle_connect(boost::system::error_code const & error)
		{
			if(! error)
			{
				on_connected_internal();
			}
			else
			{
				on_error(ErrorConnect, error);
			}
		}
	};

} }

#endif