// *********************************************************************
//  IBufferManager   version:  1.0   ¡¤  date: 09/21/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_IBUFFERMANAGER_HPP
#define LV_RPC_IBUFFERMANAGER_HPP

#include <lv/Buffer.hpp>

namespace lv { namespace rpc {


	class IBufferManager
	{
	public:

		virtual ~IBufferManager(){}

		virtual BufferPtr	get() = 0;
	};

} }

#endif // LV_RPC_IBUFFERMANAGER_HPP