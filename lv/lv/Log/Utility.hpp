// *********************************************************************
//  Utility   version:  1.0   ¡¤  date: 04/11/2009
//  --------------------------------------------------------------------
//  
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

	class CommonFormatters
	{
		bool	line_break_;

	public:
		explicit CommonFormatters(bool line_break = true)
			: line_break_(line_break)
		{
		}

		gather_ptr operator () (gather_ptr gather) const
		{
			(*gather)
				.add_header(Time())
				.add_header(Tag())
			;
			if(line_break_)
				gather->add_tailer(LineBreak());

			return gather;
		}
	};


	gather_ptr	add_gather(Log & log, ostream_ptr os, FormmatterSet formatters = CommonFormatters())
	{
		gather_ptr gather(new Gather(os));

		log.add_gather(formatters(gather));

		return gather;
	}



	/**
	 * @exception lv::file_io_error error opening the file
	 */
	gather_ptr	add_file_gather(Log & log, char const * file, bool append = true, FormmatterSet formatters = CommonFormatters())
	{
		ostream_ptr ofile(new std::basic_ofstream<char_type>(file, append ? 0 : std::ios_base::trunc));
		if(! (*ofile))
			throw file_io_error(std::string("error opening file:") + file);

		return add_gather(log, ofile, formatters);
	}


	gather_ptr	add_stdio_gather(Log & log, FormmatterSet formatters = CommonFormatters())
	{
#ifndef LV_UNICODE_LOG
		return add_gather(log, shared_from_object(std::cout), formatters);
#else
		return add_gather(log, shared_from_object(std::wcout), formatters);
#endif
	}



#ifdef LV_PLATFORM_WINDOWS

	gather_ptr	add_debug_string_gather(Log & log, FormmatterSet formatters = CommonFormatters(false))
	{
		gather_ptr gather(new DebugStringGather());
		
		log.add_gather(formatters(gather));

		return gather;
	}

#endif // LV_PLATFORM_WINDOWS

} }

#endif