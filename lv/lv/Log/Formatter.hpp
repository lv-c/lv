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

#include <lv/Log/Level.hpp>
#include <lv/Timer.hpp>

namespace lv { namespace formatter {


	// most often this's used as a tailer
	struct LineBreak
	{
		void operator () (std::ostream & os, log::level)
		{
			os << std::endl;
		}
	};

	struct Time
	{
		void operator () (std::ostream & os, log::level)
		{
			using namespace boost::posix_time;
			os << boost::format("%1%%|24t|") % to_simple_string(
				second_clock::local_time());
		}
	};

	struct Clock 
	{
		void operator () (std::ostream & os, log::level)
		{
			os << boost::format("%1%%|15t|") % (timer_.elapsed() * 1000);
		}

	private:

		Timer timer_;
	};

	struct Tag
	{
		void operator () (std::ostream & os, log::level lvl)
		{
			static std::string const tags [] = {
				"debug:", "info:", "error:", "fatal:"
			};
			os << boost::format("%1%%|10t|") % tags[lvl];
		}
	};

} }

#endif // LV_FORMATTER_HPP