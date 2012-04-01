// *********************************************************************
//  Dup   version:  1.0   ¡¤  date: 2012/03/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ZEROMQ_DUP_HPP
#define LV_ZEROMQ_DUP_HPP

#include <lv/Config.hpp>

#include <boost/assert.hpp>

#ifdef LV_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace lv { namespace zeromq {

#ifdef LV_PLATFORM_WINDOWS

	inline int	dup(int fd)
	{
		HANDLE src_handle = reinterpret_cast<HANDLE>(fd);
		HANDLE proc_handle = GetCurrentProcess();
		HANDLE target_handle = NULL;

		BOOST_VERIFY(::DuplicateHandle(proc_handle, src_handle, proc_handle, &target_handle, 0, FALSE, 0));

		return reinterpret_cast<int>(target_handle);
	}

#endif

} }

#endif