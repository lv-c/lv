// *********************************************************************
//  AutoLink   version:  1.0   ¡¤  date: 2013/05/26
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS

#	ifdef _DEBUG
#		define LV_LIB_DEBUG_OPT	"D"
#else
#		define LV_LIB_DEBUG_OPT
#	endif

#	ifdef _DLL
#		define LV_LIB_LINK_OPT
#	else
#		define LV_LIB_LINK_OPT	"S"
#	endif

#	ifdef LV_X64
#		define LV_LIB_PLATFORM_OPT	"-x64"
#	else
#		define LV_LIB_PLATFORM_OPT
#	endif

#	pragma comment(lib, LV_LIB_NAME LV_LIB_LINK_OPT LV_LIB_DEBUG_OPT LV_LIB_PLATFORM_OPT ".lib")

#endif

#undef LV_LIB_NAME
#undef LV_LIB_DEBUG_OPT
#undef LV_LIB_LINK_OPT
#undef LV_LIB_PLATFORM_OPT