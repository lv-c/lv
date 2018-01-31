// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 05/05/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>


namespace lv 
{
	namespace log 
	{

#ifndef LV_UNICODE_LOG

		using char_type = char;

#		define L_TEXT(x)	x


#else

		using char_type = wchar_t;

#		define L_TEXT(x)	L ## x

#endif

		using string_type = std::basic_string<char_type>;
		using ostream_type = std::basic_ostream<char_type>;
		
		using ostream_ptr = std::shared_ptr<ostream_type>;

		using formatter_type = std::function<void (log::ostream_type &, int)>;

		// returns true to output the record and false to suppress it
		using filter_type = std::function<bool (int)>;


		class Gather;
		using gather_ptr = std::shared_ptr<Gather>;

		class Log;

		class LogManager;

	}

	using log::Log;
	
	using log::LogManager;

}
