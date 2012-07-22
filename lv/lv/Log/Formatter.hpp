// *********************************************************************
//  Formatter   version:  1.0   ¡¤  date: 09/02/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FORMATTER_HPP
#define LV_FORMATTER_HPP

#include <iostream>

#include <boost/date_time.hpp>
#include <boost/format.hpp>

#include <lv/Log/Fwd.hpp>
#include <lv/Log/Level.hpp>
#include <lv/Timer.hpp>

namespace lv { namespace log {

#ifndef LV_UNICODE_LOG
	typedef boost::format	format_type;
#else
	typedef boost::wformat	format_type;
#endif

	// most often this's used as a tailer
	struct LineBreak
	{
		void operator () (ostream_type& os, int) const
		{
			os << std::endl;
		}
	};

	struct Time
	{
		void operator () (ostream_type & os, int) const
		{
			using namespace boost::posix_time;
			os << format_type(L_TEXT("%1%%|24t|")) % to_simple_string_type<char_type>(
				second_clock::local_time());
		}
	};

	struct Clock 
	{
		void operator () (ostream_type & os, int) const
		{
			os << format_type(L_TEXT("%f%|12t|")) % timer_.elapsed();
		}

	private:

		Timer timer_;
	};

	struct Flush
	{
		void operator () (ostream_type & os, int) const
		{
			os << std::flush;
		}
	};

	struct Tag
	{
		void operator () (ostream_type & os, int lvl) const
		{
			os << format_type(L_TEXT("[%1%]%|10t|")) % level_str(lvl);
		}
	};

} }

#endif // LV_FORMATTER_HPP
