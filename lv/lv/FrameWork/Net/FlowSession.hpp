// *********************************************************************
//  FlowSession   version:  1.0   ¡¤  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/FrameWork/Net/PacketBufferManager.hpp>
#include <lv/FrameWork/Net/PacketSplitter.hpp>
#include <lv/FrameWork/Net/Context.hpp>

#include <lv/DataFlow.hpp>

#include <lv/Serialization/IArchive.hpp>
#include <lv/Serialization/OArchive.hpp>


namespace lv::net
{
	template<class Key, class S>
	class FlowSession : public S
	{
		PacketSplitter<uint16_t>	splitter_;

	protected:

		using base_type = S;
		using key_type = Key;

		using sink_type = flow::Sink<flow::SyncPush, key_type>;
		sink_type		sink_;

		using source_type = flow::Source<key_type>;
		source_type		source_;

	public:

		/// @TODO constructor of the base_type may take other parameters
		explicit FlowSession(ContextPtr context)
			: base_type(context)
			, source_([this](BufferPtr buf) { push(buf); }, std::make_shared<PacketBufferManager>(1024))
		{
		}

		void	close() override
		{
			base_type::close();

			sink_.stop();
		}

	protected:

		void	push(BufferPtr buf)
		{
			if (buf->size() > 2)
			{
				buffer::write(*buf, 0, uint16_t(buf->size()));

				this->start_write(buf);
			}
			else
			{
				BOOST_ASSERT(false);
			}
		}

		void	on_receive(Buffer const & buf) override
		{
			splitter_.push(buf);

			while (Buffer const * packet = splitter_.get())
			{
				try
				{
					handle_packet(*packet);
				}
				catch (std::exception const & /* ex */)
				{
					BOOST_ASSERT(false);
					close();

					return;
				}
			}
		}

		/// @note override this function to handle the exceptions. If any exception is thrown
		///	 by this function, the socket will be closed
		virtual	void	handle_packet(Buffer const & buf)
		{
			sink_.push(buf);
		}

	};

}
