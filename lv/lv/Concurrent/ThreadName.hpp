// *********************************************************************
//  ThreadName   version:  1.0   ¡¤  date: 2013/01/23
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_THREADNAME_HPP
#define LV_CONCURRENT_THREADNAME_HPP

#include <lv/lvlib2.hpp>
#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS

#include <thread>

#include <Windows.h>


namespace lv
{
	// not used currently
	// void	set_thread_name(DWORD thread_id, char const * name);

	// GetThreadId does NOT exist in XP
	// void	set_thread_name(std::thread & thread, char const * name);

	void	set_current_thread_name(char const * name);
}

#endif

#endif