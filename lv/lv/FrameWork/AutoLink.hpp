// *********************************************************************
//  AutoLink   version:  1.0   ¡¤  date: 10/29/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FRAMEWORK_AUTOLINK_HPP
#define LV_FRAMEWORK_AUTOLINK_HPP

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS

#	ifdef _DEBUG
#		pragma comment(lib, "FrameworkD.lib")
#	else
#		pragma comment(lib, "Framework.lib")
#	endif

#endif


#endif