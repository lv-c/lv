// *********************************************************************
//  Console   version:  1.0   ¡¤  date: 10/16/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS

#include <lv/lvlib2.hpp>

namespace lv
{
	bool	create_console(char const * title, bool std_io = true);
}

#endif