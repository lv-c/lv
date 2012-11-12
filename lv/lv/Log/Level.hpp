// *********************************************************************
//  Level   version:  1.0   ¡¤  date: 09/02/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LOG_LEVEL_HPP
#define LV_LOG_LEVEL_HPP

#include <lv/Log/Fwd.hpp>

#include <boost/lexical_cast.hpp>

namespace lv 
{ 
	namespace log
	{

		enum level {
			debug	= 10,
			info	= 20,
			warning	= 30,
			error	= 40,
			fatal	= 50
		};

		inline string_type	level_str(int lvl)
		{
			switch(lvl)
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
				return boost::lexical_cast<string_type>(lvl);
			}
		}
	}

	using log::debug;
	using log::info;
	using log::warning;
	using log::error;
	using log::fatal;
}

#endif // LV_LEVEL_HPP
