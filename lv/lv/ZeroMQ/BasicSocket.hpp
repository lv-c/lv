// *********************************************************************
//  BasicSocket   version:  1.0   ¡¤  date: 2012/03/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ZEROMQ_BASICSOCKET_HPP
#define LV_ZEROMQ_BASICSOCKET_HPP

#include <lv/ServiceWrapper.hpp>
#include <lv/lvlib2.hpp>

#include <zmq.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assert.hpp>
#include <boost/function.hpp>

namespace lv { namespace zeromq {

	class BasicSocket
	{
		boost::shared_ptr<zmq::socket_t>	zmq_socket_;

		zmq::context_t &	context_;
		int					type_;

		ServiceWrapper	service_wrapper_;

		typedef boost::asio::ip::tcp::socket		asio_socket_type;
		typedef boost::shared_ptr<asio_socket_type>	asio_socket_ptr;

		asio_socket_ptr	asio_socket_;

		typedef boost::function<void(BasicSocket &)>	readable_callback;
		readable_callback	readable_callback_;

	public:

		BasicSocket(zmq::context_t & context, int type);

		BasicSocket(zmq::context_t & context, int type, ServiceWrapper const & service_wrapper);

		virtual ~BasicSocket();

		virtual	void	bind(std::string const & addr);

		virtual	void	connect(std::string const & addr);

		virtual	void	close();

		bool			closed() const;

		virtual	void	recreate();

		int				type() const;


		virtual	size_t	send(void const * buf, size_t len, int flags = 0)
		{
			return zmq_socket_->send(buf, len, flags);
		}

		virtual	bool	send(zmq::message_t & msg, int flags = 0)
		{
			return zmq_socket_->send(msg, flags);
		}

		virtual	size_t	recv(void * buf, size_t len, int flags = 0)
		{
			return zmq_socket_->recv(buf, len, flags);
		}

		virtual	bool	recv(zmq::message_t * msg, int flags = 0)
		{
			return zmq_socket_->recv(msg, flags);
		}


		// If you set a callback, on_socket_readable will not be called.
		void	set_readable_callback(readable_callback const & callback);

		template<typename T>
		T	getsockopt(int option_name)
		{
			BOOST_MPL_ASSERT((boost::is_integral<T>));

			T val;
			size_t size = sizeof(val);
			zmq_socket_->getsockopt(option_name, &val, &size);

			BOOST_ASSERT(size == sizeof(val));

			return val;
		}

		virtual	void	setsockopt(int option, void const * optval, size_t optvallen)
		{
			zmq_socket_->setsockopt(option, optval, optvallen);
		}

	protected:

		ServiceWrapper const &	service_wrapper() const;

		virtual	void	config_socket();

		virtual	void	on_socket_readable_internal();

		virtual	void	on_socket_readable() {}

		virtual	void	on_error(boost::system::error_code const & error) {}

		virtual	bool	is_readable();

		void	async_read();

		void	handle_receive(boost::weak_ptr<zmq::socket_t> const & weak_zmq_sock, boost::system::error_code const & error);

	};

} }

#endif