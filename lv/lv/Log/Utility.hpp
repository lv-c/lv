// *********************************************************************
//  Utility   version:  1.0   ¡¤  date: 04/11/2009
//  --------------------------------------------------------------------
//  Utilities which make the initialization of Log easier.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LOG_UTILITY_HPP
#define LV_LOG_UTILITY_HPP

#include <lv/Log/Fwd.hpp>
#include <lv/Log/Log.hpp>
#include <lv/Log/DebugStringGather.hpp>
#include <lv/Exception.hpp>

#include <fstream>
#include <boost/function.hpp>

namespace lv { namespace log {

	typedef boost::function<gather_ptr(gather_ptr)>	FormmatterSet;
	

	struct EmptyFormatters
	{
		gather_ptr operator () (gather_ptr gather) const
		{
			return gather;
		}
	};

	class CommonFormatters
	{
		bool	line_break_;

		bool	use_clock_;

	public:
		CommonFormatters(bool use_clock = false, bool line_break = true)
			: line_break_(line_break)
			, use_clock_(use_clock)
		{
		}

		gather_ptr operator () (gather_ptr gather) const
		{
			if (use_clock_)
			{
				gather->add_header(Clock());
			}
			else
			{
				gather->add_header(Time());
			}

			gather->add_header(Tag());
			
			if (line_break_)
			{
				gather->add_tailer(LineBreak());
			}

			return gather;
		}
	};


	inline gather_ptr	add_gather(Log & log, ostream_ptr os, FormmatterSet formatters = CommonFormatters())
	{
		gather_ptr gather(new Gather(os));

		log.add_gather(formatters(gather));

		return gather;
	}



	/**
	 * @exception lv::file_io_error error opening the file
	 */
	inline gather_ptr	add_file_gather(Log & log, char const * file, bool append = false, FormmatterSet formatters = CommonFormatters())
	{
		ostream_ptr ofile(new std::basic_ofstream<char_type>(file, append ? std::ios_base::app : std::ios_base::trunc));
		if (! (*ofile))
		{
			throw file_io_error(std::string("error opening file:") + file);
		}

		return add_gather(log, ofile, formatters);
	}


	inline gather_ptr	add_stdio_gather(Log & log, FormmatterSet formatters = CommonFormatters())
	{
#ifndef LV_UNICODE_LOG
		return add_gather(log, shared_from_object(std::cout), formatters);
#else
		return add_gather(log, shared_from_object(std::wcout), formatters);
#endif
	}



#ifdef LV_PLATFORM_WINDOWS

	inline gather_ptr	add_debug_string_gather(Log & log, FormmatterSet formatters = CommonFormatters(true))
	{
		gather_ptr gather(new DebugStringGather());
		
		log.add_gather(formatters(gather));

		return gather;
	}

#endif

} }

#endif
