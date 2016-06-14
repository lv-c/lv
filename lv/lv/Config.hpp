// *********************************************************************
//  Config   version:  1.0   ¡¤  date: 08/04/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_CONFIG_HPP
#define LV_CONFIG_HPP

#include <lv/IntType.hpp>


#if defined(linux) || defined(__linux) || defined(__linux__)
#	define LV_PLATFORM_LINUX

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define LV_PLATFORM_WINDOWS

#	ifdef _WIN64
#		define LV_X64
#	endif

#else
#	error "Unknown platform"

#endif

#ifndef NULL
#define NULL	0
#endif


#endif