// *********************************************************************
//  SinkSocket   version:  1.0   ¡¤  date: 2012/03/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ZEROMQ_SINKSOURCE_HPP
#define LV_ZEROMQ_SINKSOURCE_HPP

#include <lv/ZeroMQ/Fwd.hpp>
#include <lv/ZeroMQ/BasicSocket.hpp>
#include <lv/DataFlow/Sink.hpp>
#include <lv/DataFlow/SyncPush.hpp>

namespace lv { namespace zeromq {

	template<class Key, class IArchive>
	class SinkSocket : public BasicSocket, public flow::Sink<flow::SyncPush, Key, IArchive>
	{
	public:

		SinkSocket(zmq::context_t & context, int type, ServiceWrapper const & service_wrapper,
			flow::proxy_push_type const & proxy_push = flow::proxy_push_type())
			: BasicSocket(context, type, service_wrapper)
			, flow::Sink<flow::SyncPush, Key, IArchive>(proxy_push)
		{
		}

	protected:

		virtual	void	on_socket_readable()
		{
			zmq::message_t msg;

			if(socket().recv(&msg))
			{
				this->push_impl(ConstBufferRef(static_cast<char const*>(msg.data()), msg.size()));
			}
		}
	};
	
} }

#endif