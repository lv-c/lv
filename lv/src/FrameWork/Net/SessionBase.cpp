#include <lv/FrameWork/Net/SessionBase.hpp>
#include <lv/FrameWork/Net/SocketHolder.hpp>
#include <lv/FrameWork/Net/Context.hpp>

#include <boost/asio/write.hpp>
#include <boost/asio/placeholders.hpp>


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
		, context_(context)
	{
	}

	SessionBase::~SessionBase() = default;

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

	CloseEvent & SessionBase::close_event()
	{
		return close_event_;
	}


	void SessionBase::shutdown()
	{
		write_buffers_.shutdown();
		check_write();
	}

	void SessionBase::close()
	{
		if (!closed_.exchange(true))
		{
			boost::system::error_code error;
			socket_->get().close(error);

			on_closed();
		}
	}

	bool SessionBase::closed()
	{
		return closed_;
	}

	void SessionBase::connect(std::string const & ip, std::string const & port, std::string const & to_bind /* = std::string */)
	{
		closed_ = false;
		write_buffers_.reset();

		asio::ip::tcp::resolver::query query(ip, port);
		asio::ip::tcp::resolver resolver(context_->io_context());

		if (!to_bind.empty())
		{
			asio::ip::address bind_addr = asio::ip::address::from_string(to_bind);

			socket_->get().open(bind_addr.is_v4() ? asio::ip::tcp::v4() : asio::ip::tcp::v6());
			socket_->get().bind(asio::ip::tcp::endpoint(bind_addr, 0));
		}

		auto handler = [shared_this = shared_from_this()](boost::system::error_code const & error) {
			shared_this->handle_connect(error);
		};

		if (context_->has_strand())
		{
			socket_->get().async_connect(*resolver.resolve(query), context_->strand().wrap(std::move(handler)));
		}
		else
		{
			socket_->get().async_connect(*resolver.resolve(query), std::move(handler));
		}
	}

	void SessionBase::start_read(BufferPtr buf)
	{
		if (buf && buf->size() > max_buffer_size_)
		{
			buf.reset();
		}

		if (!buf)
		{
			buf = std::make_shared<Buffer>(20 * 1024);
		}

		buf->resize(buf->capacity());

		auto handler = [shared_this = shared_from_this(), buf](boost::system::error_code const & error, size_t bytes_transferred) {
			shared_this->handle_read(buf, bytes_transferred, error);
		};

		if (context_->has_strand())
		{
			socket_->get().async_read_some(asio::buffer(*buf), context_->strand().wrap(std::move(handler)));
		}
		else
		{
			socket_->get().async_read_some(asio::buffer(*buf), std::move(handler));
		}
	}

	void SessionBase::start_write(ConstBufferRef buf)
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

			write_buffers_.put(buf);

			check_write();
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
		start_read(BufferPtr());
		on_connected();
	}

	void SessionBase::on_receive_internal(Buffer const & buf)
	{
		on_receive(buf);
	}

	void SessionBase::on_connected()
	{
		connect_event_();
	}

	void SessionBase::on_receive(Buffer const & buf)
	{
		receive_event_(buf);
	}

	void SessionBase::on_write(size_t size)
	{
		write_event_(size);
	}

	void SessionBase::on_closed()
	{
		close_event_();
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
			on_receive_internal(*buf);

			//
			start_read(buf);
		}
	}

	void SessionBase::handle_write(Buffer const & buf, boost::system::error_code const & error)
	{
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
			size_t size = buf.size();

			write_buffers_.unlock(buf);
			check_write();

			on_write(size);
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

	void SessionBase::check_write()
	{
		bool need_shutdown;

		if (Buffer const * buf = write_buffers_.lock(&need_shutdown))
		{
			auto handler = [shared_this = shared_from_this(), buf](boost::system::error_code const & error, size_t) {
				return shared_this->handle_write(*buf, error);
			};

			if (context_->has_strand())
			{
				asio::async_write(socket_->get(), asio::buffer(*buf), context_->strand().wrap(std::move(handler)));
			}
			else
			{
				asio::async_write(socket_->get(), asio::buffer(*buf), std::move(handler));
			}
		}
		else
		{
			if (need_shutdown)
			{
				boost::system::error_code error;
				socket_->get().shutdown(asio::ip::tcp::socket::shutdown_both, error);
			}
		}
	}
}
