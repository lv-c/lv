// *********************************************************************
//  PacketBufferManager   version:  1.0   ¡¤  date: 01/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_PACKETBUFFERMANAGER_HPP
#define LV_NET_PACKETBUFFERMANAGER_HPP

#include <lv/SimpleBufferManager.hpp>

namespace lv { namespace net {

	class PacketBufferManager : public SimpleBufferManager
	{
	public:

		explicit PacketBufferManager(size_t init_capacity)
			: lv::SimpleBufferManager(init_capacity)
		{
		}

		virtual BufferPtr	get()
		{
			BufferPtr buf = lv::SimpleBufferManager::get();

			// used to store the size of this packet
			buf->assign(2, 0);

			return buf;
		}
	};

} }

#endif