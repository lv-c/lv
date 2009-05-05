// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 05/05/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LOG_FWD_HPP
#define LV_LOG_FWD_HPP

#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>

#include <lv/Log/Level.hpp>

namespace lv 
{
	namespace log 
	{

#ifndef LV_UNICODE_LOG

		typedef	char			char_type;

#		define L_TEXT(x)	x


#else

		typedef	wchar_t			char_type;

#		define L_TEXT(x)	L ## x

#endif

		typedef std::basic_string<char_type>	string_type;
		typedef std::basic_ostream<char_type>	ostream_type;
		
		typedef boost::shared_ptr<ostream_type>	ostream_ptr;

		typedef boost::function<void (log::ostream_type &, log::level)> formatter_type;

		// returns true to output the record and false to suppress it
		typedef boost::function<bool (log::level)>	filter_type;


		class Gather;
		typedef boost::shared_ptr<Gather>	gather_ptr;

		class Log;

		class LogManager;

	}

	using log::Log;
	
	using log::LogManager;

	using log::debug;
	using log::info;
	using log::warning;
	using log::error;
	using log::fatal;
}


#endif // LV_LOG_FWD_HPP