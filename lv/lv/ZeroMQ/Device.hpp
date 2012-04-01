// *********************************************************************
//  Device   version:  1.0   ¡¤  date: 2012/03/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ZEROMQ_DEVICE_HPP
#define LV_ZEROMQ_DEVICE_HPP

#include <lv/ZeroMQ/Fwd.hpp>

#include <boost/array.hpp>
#include <boost/noncopyable.hpp>

namespace lv { namespace zeromq {

	class Device : boost::noncopyable
	{
		boost::array<BasicSocket*, 2>	sockets_;

	public:

		Device();

		void	start(BasicSocket & sock1, BasicSocket & sock2);

	private:

		void	on_socket_readable(size_t index);
	};

} }

#endif