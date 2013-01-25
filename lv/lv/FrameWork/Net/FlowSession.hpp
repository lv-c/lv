// *********************************************************************
//  FlowSession   version:  1.0   ¡¤  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_FLOWSESSION_HPP
#define LV_NET_FLOWSESSION_HPP

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/FrameWork/Net/PacketBufferManager.hpp>
#include <lv/FrameWork/Net/PacketSplitter.hpp>
#include <lv/FrameWork/Net/Context.hpp>

#include <lv/DataFlow.hpp>

#include <lv/Serialization/IArchive.hpp>
#include <lv/Serialization/OArchive.hpp>


namespace lv { namespace net {

	template<typename Key, class S>
	class FlowSession : public S
	{
		PacketSplitter<uint16>	splitter_;

	protected:

		typedef S	base_type;
		typedef Key	key_type;

		typedef flow::Sink<flow::SyncPush, key_type> sink_type;
		sink_type	sink_;

		typedef flow::Source<key_type>	source_type;
		boost::shared_ptr<source_type>	source_;

	public:

		/// @TODO constructor of the base_type may take other parameters
		explicit FlowSession(ContextPtr context)
			: base_type(context)
			, splitter_(context->buffer_manager())
		{
			source_.reset(new source_type(boost::bind(&FlowSession::push, this, _1), 
				BufferManagerPtr(new PacketBufferManager(1024))));
		}

	protected:

		void	push(BufferPtr buf)
		{
			if(buf->size() > 2)
			{
				buffer::write(buf, 0, uint16(buf->size()));

				start_write(buf);
			}
			else
			{
				BOOST_ASSERT(false);
			}
		}

		virtual void	on_receive(BufferPtr buf)
		{
			splitter_.push(buf);

			while(true)
			{
				BufferPtr new_buf = splitter_.get();
				if(! new_buf)
				{
					break;
				}

				try
				{
					handle_packet(new_buf);
				}
				catch (std::exception const & /* ex */)
				{
					BOOST_ASSERT(false);
					close();

					return;
				}
			}
		}

		/// @note overload this function to handle the exceptions. If any exception is thrown
		///	 by this function, the socket will be closed
		virtual	void	handle_packet(BufferPtr buf)
		{
			sink_.push(buf);
		}

	};

} }

#endif