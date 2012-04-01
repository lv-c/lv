// *********************************************************************
//  SourceSocket   version:  1.0   ¡¤  date: 2012/03/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ZEROMQ_SOURCESOCKET_HPP
#define LV_ZEROMQ_SOURCESOCKET_HPP

#include <lv/ZeroMQ/Fwd.hpp>
#include <lv/ZeroMQ/BasicSocket.hpp>
#include <lv/DataFlow/Source.hpp>
#include <lv/SimpleBufferManager.hpp>

#include <cstring>

namespace lv { namespace zeromq {

	template<class Key, class OArchive>
	class SourceSocket : public BasicSocket, public flow::Source<Key, OArchive>
	{
		typedef flow::Source<Key, OArchive>	source_type;

	public:

		SourceSocket(zmq::context_t & context, int type)
			: BasicSocket(context, type)
			, source_type(source_type::callback_type(), BufferManagerPtr(new SimpleBufferManager(1024)))
		{
			source_type::set_callback(boost::bind(&SourceSocket::push, this, _1));
		}

	protected:

		virtual	void	push(BufferPtr const & buf)
		{
			zmq::message_t msg(buf->size());
			std::memcpy(msg.data(), buffer::data(buf), buf->size());

			socket().send(msg);
		}
	};

} }

#endif