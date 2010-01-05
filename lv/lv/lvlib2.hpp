// *********************************************************************
//  lvlib2   version:  1.0   ¡¤  date: 01/05/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LVLIB2_HPP
#define LV_LVLIB2_HPP

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS

#	ifdef _DEBUG
#		pragma comment(lib, "lv2d.lib")
#	else
#		pragma comment(lib, "lv2.lib")
#	endif

#endif

#endif