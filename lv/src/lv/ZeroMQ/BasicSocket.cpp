#include <lv/ZeroMQ/BasicSocket.hpp>
#include <lv/ZeroMQ/Dup.hpp>
#include <lv/IntType.hpp>

#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>

namespace lv { namespace zeromq {

	BasicSocket::BasicSocket(zmq::context_t & context, int type)
		: context_(context)
		, type_(type)
	{
		recreate();
	}

	BasicSocket::BasicSocket(zmq::context_t & context, int type, ServiceWrapper const & service_wrapper)
		: context_(context)
		, type_(type)
		, service_wrapper_(service_wrapper)
	{
		recreate();
	}

	void BasicSocket::bind(std::string const & addr)
	{
		zmq_socket_->bind(addr.c_str());
		config_socket();
	}

	void BasicSocket::connect(std::string const & addr)
	{
		zmq_socket_->connect(addr.c_str());
		config_socket();
	}

	void BasicSocket::config_socket()
	{
		if(type_ == ZMQ_SUB)
		{
			zmq_socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
		}

		if(is_readable())
		{
			async_read();
		}
	}

	bool BasicSocket::is_readable()
	{
		switch(type_)
		{
		case ZMQ_SUB:
		case ZMQ_REQ:
		case ZMQ_REP:
		case ZMQ_DEALER:
		case ZMQ_ROUTER:
		case ZMQ_PULL:
		case ZMQ_XPUB:
		case ZMQ_XSUB:
			return true;

		default:
			return false;
		}
	}

	void BasicSocket::close()
	{
		zmq_socket_.reset();
		asio_socket_.reset();
	}

	bool BasicSocket::closed()
	{
		return zmq_socket_;
	}

	void BasicSocket::recreate()
	{
		close();
		zmq_socket_.reset(new zmq::socket_t(context_, type_));
	}

	zmq::socket_t & BasicSocket::socket()
	{
		return *zmq_socket_;
	}

	int BasicSocket::type()
	{
		return type_;
	}

	void BasicSocket::set_readable_callback(readable_callback const & callback)
	{
		readable_callback_ = callback;

		if(is_readable())
		{
			async_read();
		}
	}

	void BasicSocket::async_read()
	{
		if(! asio_socket_)
		{
			int fd = getsockopt<int32>(ZMQ_FD);
			fd = zeromq::dup(fd);

			asio_socket_.reset(new asio_socket_type(service_wrapper_.service(), boost::asio::ip::tcp::v4(),
				static_cast<asio_socket_type::native_type>(fd)));
		}

		// io_service poller is edge-triggered and will not react if the socket is already readable.
		service_wrapper_.post(boost::bind(&BasicSocket::handle_receive, this, 
			boost::weak_ptr<zmq::socket_t>(zmq_socket_), boost::system::error_code()));
	}

	void BasicSocket::handle_receive(boost::weak_ptr<zmq::socket_t> const & weak_zmq_sock, boost::system::error_code const & error)
	{
		if(boost::shared_ptr<zmq::socket_t> zmq_sock = weak_zmq_sock.lock())
		{
			if(error)
			{
				on_error(error);
				return;
			}

			while(true)
			{
				int32 events = getsockopt<int32>(ZMQ_EVENTS);

				if(events & ZMQ_POLLIN)
				{
					if(readable_callback_)
					{
						readable_callback_(*zmq_socket_);
					}
					else
					{
						on_socket_readable();
					}
				}
				else
				{
					break;
				}
			}

			if(service_wrapper_.has_strand())
			{
				asio_socket_->async_read_some(boost::asio::null_buffers(), service_wrapper_.strand().wrap(boost::bind(
					&BasicSocket::handle_receive, this, weak_zmq_sock, boost::asio::placeholders::error)));
			}
			else
			{
				asio_socket_->async_read_some(boost::asio::null_buffers(), boost::bind(
					&BasicSocket::handle_receive, this, weak_zmq_sock, boost::asio::placeholders::error));
			}
		}
	}

} }