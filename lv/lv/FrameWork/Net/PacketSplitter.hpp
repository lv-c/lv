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

#include <lv/Endian.hpp>
#include <lv/Buffer.hpp>

#include <deque>


namespace lv::net
{
	template<class SizeType>
	class PacketSplitter
	{
		std::deque<char>	cache_;

		Buffer				buffer_;

		bool				switch_endian_;

	public:

		explicit PacketSplitter(bool switch_endian = false)
			: switch_endian_(switch_endian)
		{
		}

		void	push(ConstBufferRef buf)
		{
			cache_.insert(cache_.end(), buf.begin(), buf.end());
		}

		void	clear()
		{
			cache_.clear();
		}

		Buffer *	get()
		{
			size_t const header_size = sizeof(SizeType);

			if (header_size <= cache_.size())
			{
				SizeType size;	// includes size of the header
				std::copy(cache_.begin(), cache_.begin() + header_size, reinterpret_cast<char*>(&size));

				if (switch_endian_)
				{
					size = lv::endian_switch(size);
				}

				if (size <= cache_.size())
				{
					buffer_.assign(cache_.begin() + header_size, cache_.begin() + size);
					cache_.erase(cache_.begin(), cache_.begin() + size);

					return &buffer_;
				}
			}

			return nullptr;
		}


		std::deque<char> const &	remaining() const
		{
			return cache_;
		}
		
	};

}