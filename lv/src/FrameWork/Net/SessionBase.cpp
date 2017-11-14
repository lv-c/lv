#include <lv/FrameWork/Net/SessionBase.hpp>
#include <lv/FrameWork/Net/SocketHolder.hpp>
#include <lv/FrameWork/Net/Context.hpp>

#include <boost/asio/write.hpp>
#include <boost/asio/placeholders.hpp>

#include <functional>


namespace lv::net
{
	char const * error_string(ErrorType type)
	{
		switch (type)
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

	SessionBase::SessionBase(ContextPtr context, SocketHolderPtr socket)
		: socket_(socket)
		, closed_(false)
		, writing_(false)
		, context_(context)
	{
	}

	SessionBase::~SessionBase()
	{
	}

	std::string SessionBase::remote_ip()
	{
		return get_ip(true);
	}

	std::string SessionBase::local_ip()
	{
		return get_ip(false);
	}

	std::string SessionBase::get_ip(bool remote)
	{
		boost::system::error_code error;
		asio::ip::tcp::endpoint endpoint = remote ?
			socket_->get().remote_endpoint(error) : socket_->get().local_endpoint(error);

		if (!error)
		{
			return endpoint.address().to_string(error);
		}

		return std::string();
	}

	SocketHolderPtr SessionBase::socket()
	{
		return socket_;
	}

	ErrorEvent & SessionBase::error_event()
	{
		return error_event_;
	}

	ConnectEvent & SessionBase::connect_event()
	{
		return connect_event_;
	}

	ReceiveEvent & SessionBase::receive_event()
	{
		return receive_event_;
	}

	WriteEvent & SessionBase::write_event()
	{
		return write_event_;
	}


	void SessionBase::shutdown()
	{
		boost::system::error_code error;
		socket_->get().shutdown(asio::ip::tcp::socket::shutdown_both, error);
	}

	void SessionBase::close()
	{
		closed_ = true;

		boost::system::error_code error;
		socket_->get().close(error);
	}

	bool SessionBase::closed()
	{
		return closed_;
	}

	void SessionBase::connect(std::string const & ip, std::string const & port, std::string const & to_bind /* = std::string */)
	{
		closed_ = false;
		writing_ = false;

		asio::ip::tcp::resolver::query query(ip, port);
		asio::ip::tcp::resolver resolver(context_->service());

		if (!to_bind.empty())
		{
			asio::ip::address bind_addr = asio::ip::address::from_string(to_bind);

			socket_->get().open(bind_addr.is_v4() ? asio::ip::tcp::v4() : asio::ip::tcp::v6());
			socket_->get().bind(asio::ip::tcp::endpoint(bind_addr, 0));
		}

		if (context_->has_strand())
		{
			socket_->get().async_connect(*resolver.resolve(query), 
				context_->strand().wrap(std::bind(&SessionBase::handle_connect, shared_from_this(), std::placeholders::_1)));
		}
		else
		{
			socket_->get().async_connect(*resolver.resolve(query), 
				std::bind(&SessionBase::handle_connect, shared_from_this(), std::placeholders::_1));
		}
	}

	void SessionBase::start_read()
	{
		BufferPtr buf = context_->buffer();

		buf->resize(buf->capacity());

		if (context_->has_strand())
		{
			socket_->get().async_read_some(asio::buffer(*buf), context_->strand().wrap(
				std::bind(&SessionBase::handle_read, shared_from_this(), buf,
				std::placeholders::_2, std::placeholders::_1)));
		}
		else
		{
			socket_->get().async_read_some(asio::buffer(*buf), 
				std::bind(&SessionBase::handle_read, shared_from_this(), buf,
				std::placeholders::_2, std::placeholders::_1));
		}
	}

	void SessionBase::start_write(BufferPtr buf)
	{
		if (!closed_)
		{
			/*
			async_write

			This operation is implemented in terms of zero or more calls to the stream's async_write_some function, 
			and is known as a composed operation. The program must ensure that the stream performs no other write
			operations (such as async_write, the stream's async_write_some function, or any other composed operations
			that perform writes) until this operation completes.
			*/

			lock_guard lock(write_mutex_);

			if (writing_)
			{
				write_queue_.push_back(buf);
			}
			else
			{
				write_impl(buf);
			}
		}
	}

	void SessionBase::write_impl(BufferPtr buf)
	{
		BOOST_ASSERT(!writing_);
		writing_ = true;

		if (context_->has_strand())
		{
			asio::async_write(socket_->get(), asio::buffer(*buf), context_->strand().wrap(
				std::bind(&SessionBase::handle_write, shared_from_this(), buf, std::placeholders::_1)));
		}
		else
		{
			asio::async_write(socket_->get(), asio::buffer(*buf), 
				std::bind(&SessionBase::handle_write, shared_from_this(), buf, std::placeholders::_1));
		}
	}

	void SessionBase::server_side_start()
	{
		on_connected_internal();
	}


	void SessionBase::on_error(ErrorType type, boost::system::error_code const & error)
	{
		error_event_(type, error);
	}

	void SessionBase::on_error_internal(ErrorType type, boost::system::error_code const & error)
	{
		if (!closed_)
		{
			close();
			on_error(type, error);
		}
	}

	void SessionBase::on_connected_internal()
	{
		start_read();
		on_connected();
	}

	void SessionBase::on_receive_internal(BufferPtr buf)
	{
		on_receive(buf);
	}

	void SessionBase::on_connected()
	{
		connect_event_();
	}

	void SessionBase::on_receive(BufferPtr buf)
	{
		receive_event_(buf);
	}

	void SessionBase::on_write(BufferPtr buf)
	{
		write_event_(buf);
	}

	void SessionBase::handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error)
	{
		if (closed_)
		{
			return;
		}

		if (error)
		{
			on_error_internal(ErrorRead, error);
		}
		else
		{
			buf->resize(bytes_transferred);
			on_receive_internal(buf);

			start_read();
		}
	}

	void SessionBase::handle_write(BufferPtr buf, boost::system::error_code const & error)
	{
		writing_ = false;
		
		//
		if (closed_)
		{
			return;
		}

		if (error)
		{
			on_error_internal(ErrorWrite, error);
		}
		else
		{
			{
				lock_guard lock(write_mutex_);

				if (!writing_ && !write_queue_.empty())
				{
					BufferPtr new_buf = write_queue_.front();
					write_queue_.pop_front();

					write_impl(new_buf);
				}
			}

			on_write(buf);
		}
	}

	void SessionBase::handle_connect(boost::system::error_code const & error)
	{
		if (closed_)
		{
			return;
		}

		if (!error)
		{
			on_connected_internal();
		}
		else
		{
			on_error_internal(ErrorConnect, error);
		}
	}

}
