// *********************************************************************
//  Level   version:  1.0   ¡¤  date: 09/02/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Log/Fwd.hpp>


namespace lv 
{ 
	namespace log
	{
		enum level
		{
			debug	= 10,
			info	= 20,
			warning	= 30,
			error	= 40,
			fatal	= 50
		};

		inline string_type	level_str(int lvl)
		{
			switch (lvl)
			{
			case debug:
				return L_TEXT("debug");

			case info:
				return L_TEXT("info");

			case warning:
				return L_TEXT("warning");

			case error:
				return L_TEXT("error");

			case fatal:
				return L_TEXT("fatal");

			default:

#ifdef LV_UNICODE_LOG
				return std::to_wstring(lvl);
#else
				return std::to_string(lvl);
#endif
			}
		}
	}

	using log::debug;
	using log::info;
	using log::warning;
	using log::error;
	using log::fatal;
}
