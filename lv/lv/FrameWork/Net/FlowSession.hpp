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

#include <lv/DataFlow.hpp>

#include <lv/Serialization/IArchive.hpp>
#include <lv/Serialization/OArchive.hpp>


namespace lv { namespace net {

	template<typename Key, class S>
	class FlowSession : public S
	{
	protected:

		typedef S	base_type;
		typedef Key	key_type;

		typedef flow::Sink<flow::SyncPush, key_type> sink_type;
		sink_type	sink_;

		typedef flow::Source<key_type, int>	source_type;
		boost::shared_ptr<source_type>	source_;

		Buffer	cache_;


	public:

		/// @TODO constructor of the base_type may take other parameters
		FlowSession(ContextPtr context)
			: base_type(context)
		{
			source_.reset(new source_type(0, boost::bind(&FlowSession::push, this, _1, _2), 
				BufferManagerPtr(new PacketBufferManager(1024))));
		}

	protected:

		void	push(int const & port, BufferPtr buf)
		{
			if(buf->size() > 2)
			{
				*reinterpret_cast<uint16*>(&(*buf)[0]) = static_cast<uint16>(buf->size() - 2);

				start_write(buf);
			}
			else
			{
				BOOST_ASSERT(false);
			}
		}

		virtual void	on_receive(BufferPtr buf)
		{
			cache_.insert(cache_.end(), buf->begin(), buf->end());

			size_t index = 0;
			while(index + 2 < cache_.size())
			{
				uint16 size = *reinterpret_cast<uint16*>(&cache_[index]);

				if(index + 2 + size > cache_.size())
					break;

				BufferPtr newbuf = context_->buffer();
				newbuf->assign(cache_.begin() + index + 2, cache_.begin() + index + 2 + size);

				index += 2 + size;

				try
				{
					handle_packet(newbuf);
				}
				catch (std::exception const & /* ex */)
				{
					BOOST_ASSERT(false);
					close();

					return;
				}
			}

			cache_.erase(cache_.begin(), cache_.begin() + index);

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