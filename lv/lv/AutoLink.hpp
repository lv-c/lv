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
#		ifdef _DLL
#			pragma comment(lib, LV_LIB_NAME "D.lib")
#		else
#			pragma comment(lib, LV_LIB_NAME "SD.lib")
#		endif
#	else
#		ifdef _DLL
#			pragma comment(lib, LV_LIB_NAME ".lib")
#		else
#			pragma comment(lib, LV_LIB_NAME "S.lib")
#		endif
#	endif

#endif

#undef LV_LIB_NAME