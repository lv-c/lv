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


#if defined(linux) || defined(__linux) || defined(__linux__)

#	define LV_PLATFORM_LINUX

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#	define LV_PLATFORM_WINDOWS

#	define _HAS_AUTO_PTR_ETC 1


#	ifdef _WIN64
#		define LV_WIN64
#	else
#		define LV_WIN32
#	endif

#else

#	error "Unknown platform"

#endif


#endif
