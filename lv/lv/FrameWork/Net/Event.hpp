// *********************************************************************
//  Event   version:  1.0   ��  date: 2014/03/18
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_EVENT_HPP
#define LV_NET_EVENT_HPP

#include <lv/Buffer.hpp>

#include <boost/signals2/signal.hpp>
#include <boost/system/system_error.hpp>


namespace lv::net
{
	using ErrorEvent = boost::signals2::signal<void(ErrorType, boost::system::error_code const &)>;
	using ConnectEvent = boost::signals2::signal<void()>;
	using ReceiveEvent = boost::signals2::signal<void(BufferPtr)>;
	using WriteEvent = boost::signals2::signal<void(BufferPtr)>;

}

#endif