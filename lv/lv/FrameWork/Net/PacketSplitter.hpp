// *********************************************************************
//  PacketSplitter   version:  1.0   ¡¤  date: 09/28/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/IBufferManager.hpp>
#include <lv/Endian.hpp>

#include <deque>


namespace lv::net
{
	template<class SizeType>
	class PacketSplitter
	{
		std::deque<char>	cache_;

		BufferManagerPtr	buf_manager_;

		bool				switch_endian_;

	public:

		explicit PacketSplitter(BufferManagerPtr buf_manager, bool switch_endian = false)
			: buf_manager_(buf_manager)
			, switch_endian_(switch_endian)
		{
		}

		~PacketSplitter()
		{
			BOOST_ASSERT(cache_.empty());
		}

		void	push(ConstBufferRef buf)
		{
			cache_.insert(cache_.end(), buf.begin(), buf.end());
		}

		void	clear()
		{
			cache_.clear();
		}

		BufferPtr	get()
		{
			size_t const header_size = sizeof(SizeType);

			if (header_size <= cache_.size())
			{
				SizeType size;	// includes the size of the header
				std::copy(cache_.begin(), cache_.begin() + header_size, reinterpret_cast<char*>(&size));

				if (switch_endian_)
				{
					size = lv::endian_switch(size);
				}

				if (size <= cache_.size())
				{
					BufferPtr buf = buf_manager_->get();
					buf->assign(cache_.begin() + header_size, cache_.begin() + size);
					
					cache_.erase(cache_.begin(), cache_.begin() + size);

					return buf;
				}
			}

			return BufferPtr();
		}
		
	};

}