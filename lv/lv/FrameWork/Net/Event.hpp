// *********************************************************************
//  Event   version:  1.0   ¡¤  date: 2014/03/18
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Buffer.hpp>

#include <boost/signals2/signal.hpp>
#include <boost/system/system_error.hpp>


namespace lv::net
{
	using ErrorEvent = boost::signals2::signal<void(ErrorType, boost::system::error_code const &)>;

	using ConnectEvent = boost::signals2::signal<void()>;

	using ReceiveEvent = boost::signals2::signal<void(Buffer const &)>;

	using WriteEvent = boost::signals2::signal<void(size_t)>;

	using CloseEvent = boost::signals2::signal<void()>;

}