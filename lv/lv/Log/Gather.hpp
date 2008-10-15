// *********************************************************************
//  Gather   version:  1.0   ¡¤  date: 09/02/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GATHER_HPP
#define LV_GATHER_HPP

#include <iostream>
#include <list>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <lv/Log/Level.hpp>
#include <lv/Foreach.hpp>

namespace lv
{
	class Gather
	{
		friend class Log;
	public:
		typedef boost::function<void (std::ostream &, log::level)> formatter_t;

		typedef boost::shared_ptr<std::ostream>	ostream_ptr;

		Gather(ostream_ptr os, log::level log_lvl)
			: os_(os)
			, log_lvl_(log_lvl_)
		{
		}

		
		Gather &	add_header(formatter_t header)
		{
			this->headers_.push_back(header);
			return *this;
		}

		Gather &	add_tailer(formatter_t tailer)
		{
			this->tailers_.push_back(tailer);
			return *this;
		}

		log::level log_level() const
		{
			return this->log_lvl_;
		}

		
		/**
		 * Make a copy of this object with a different output stream and log level
		 * so that these two objects have the same headers and tailers.
		 */
		std::auto_ptr<Gather> clone(ostream_ptr os, log::level log_lvl)
		{
			std::auto_ptr<Gather> gather(new Gather(*this));
			gather->os_ = os;
			gather->log_lvl_ = log_lvl;

			return gather;
		}

	private:

		template <typename T>
		void log(T const & t)
		{
			*os_ << t;
		}


		void on_record_begin()
		{
			foreach(formatter_t & fmt, headers_)
			{
				fmt(*os_);
			}
		}

		void on_record_end()
		{
			foreach(formatter_t & fmt, tailers_)
			{
				fmt(*os_);
			}
		}

	private:

		log::level const	log_lvl_;

		std::list<formatter_t>	headers_;
		std::list<formatter_t>	tailers_;

		ostream_ptr	os_;
	};
}

#endif // LV_GATHER_HPP
