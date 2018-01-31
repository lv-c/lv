// *********************************************************************
//  PacketBufferManager   version:  1.0   ¡¤  date: 01/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/SimpleBufferManager.hpp>


namespace lv::net
{
	class PacketBufferManager : public SimpleBufferManager
	{
		using base_type = SimpleBufferManager;

	public:

		explicit PacketBufferManager(size_t init_capacity)
			: base_type(init_capacity)
		{
		}

		BufferPtr	get() override
		{
			BufferPtr buf = base_type::get();

			// used to store the size of this packet
			buf->assign(2, 0);

			return buf;
		}
	};

}