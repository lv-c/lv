// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 11/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_FWD_HPP
#define LV_RPC_FWD_HPP

#include <boost/shared_ptr.hpp>

namespace lv { namespace rpc {

	class ISocket;
	typedef boost::shared_ptr<ISocket> ISocketPtr;

	class IBufferManager;
	typedef boost::shared_ptr<IBufferManager> IBufferManagerPtr;
} }

#endif // LV_RPC_FWD_HPP