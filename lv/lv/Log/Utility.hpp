// *********************************************************************
//  Utility   version:  1.0   ¡¤  date: 04/11/2009
//  --------------------------------------------------------------------
//  Utilities which make the initialization of Log easier.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Log/Fwd.hpp>
#include <lv/Log/Log.hpp>
#include <lv/Log/DebugStringGather.hpp>
#include <lv/Exception.hpp>
#include <lv/SharedPtr.hpp>

#include <fstream>
#include <functional>


namespace lv::log
{
	using FormmatterSet = std::function<void(Gather &)>;
	

	struct EmptyFormatters
	{
		void	operator () (Gather &) const
		{
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

		void operator () (Gather & gather) const
		{
			if (use_clock_)
			{
				gather.add_header(Clock());
			}
			else
			{
				gather.add_header(Time());
			}

			gather.add_header(Tag());
			
			if (line_break_)
			{
				gather.add_tailer(LineBreak());
			}
		}
	};


	inline gather_ptr	add_gather(Log & log, ostream_ptr os, FormmatterSet formatters = CommonFormatters())
	{
		gather_ptr gather = std::make_shared<Gather>(os);
		formatters(*gather);

		log.add_gather(gather);

		return gather;
	}



	/**
	 * @exception lv::file_io_error error opening the file
	 */
	inline gather_ptr	add_file_gather(Log & log, char const * file, bool append = false, FormmatterSet formatters = CommonFormatters())
	{
		ostream_ptr ofile = std::make_shared<std::basic_ofstream<char_type> >(file, append ? std::ios_base::app : std::ios_base::trunc);
		if (!(*ofile))
		{
			throw file_io_error(std::string("error opening file: ") + file);
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
		gather_ptr gather = std::make_shared<DebugStringGather>();
		formatters(*gather);

		log.add_gather(gather);

		return gather;
	}

#endif

}
