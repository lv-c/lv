// *********************************************************************
//  Console   version:  1.0   ¡¤  date: 10/16/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONSOLE_HPP
#define LV_CONSOLE_HPP

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS
#include <string>

namespace lv
{
	bool	create_console(std::string const & title, bool std_io = true);
}

#endif // LV_PLATFORM_WINDOWS

#endif // LV_CONSOLE_HPP