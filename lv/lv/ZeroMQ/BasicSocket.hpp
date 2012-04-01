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

		typedef boost::function<void(zmq::socket_t &)>	readable_callback;
		readable_callback	readable_callback_;

	public:

		BasicSocket(zmq::context_t & context, int type);

		BasicSocket(zmq::context_t & context, int type, ServiceWrapper const & service_wrapper);

		virtual	void	bind(std::string const & addr);

		virtual	void	connect(std::string const & addr);

		virtual	void	close();

		bool			closed();

		virtual	void	recreate();

		zmq::socket_t &	socket();

		int				type();

		// If you set a callback, on_socket_readable will never be called.
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

	protected:

		virtual	void	config_socket();

		virtual	void	on_socket_readable() {}

		virtual	void	on_error(boost::system::error_code const & error) {}

		virtual	bool	is_readable();

		void	async_read();

		void	handle_receive(boost::weak_ptr<zmq::socket_t> const & weak_zmq_sock, boost::system::error_code const & error);

	};

} }

#endif