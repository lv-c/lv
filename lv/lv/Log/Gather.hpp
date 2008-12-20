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
#include <lv/StreamPtr.hpp>

namespace lv
{

	namespace log
	{
		// filters
		
		template<class Pred>
		class UnaryFilter
		{
			Pred	pred_;

			log::level	lvl_;
		public:
			UnaryFilter(log::level lvl, Pred pred = Pred()) 
				: lvl_(lvl)
				, pred_(pred)
			{
			}
			
			bool operator () (log::level l) const
			{ 
				return pred_(l, lvl_); 
			}
		};

		typedef UnaryFilter<std::equal_to<log::level> >			EqualFilter;
		typedef UnaryFilter<std::greater_equal<log::level> >	GreaterEqualFilter;
		typedef UnaryFilter<std::less_equal<log::level> >		LessEualFilter;

		// do not output debug messages in release version
		struct DebugFilter
		{
			bool operator () (log::level l) const
			{
#ifdef _DEBUG
				return true;
#else
				return l > log::debug;
#endif
			}
		};
	}

	// fwd
	class Gather;
	typedef std::auto_ptr<Gather>	GatherPtr;

	class Gather
	{
	public:

		typedef boost::shared_ptr<std::ostream>	ostream_ptr;

		typedef boost::function<void (std::ostream &, log::level)> formatter_t;

		// returns true to output the record and false to suppress it
		typedef boost::function<bool (log::level)>	filter_t;

	protected:

		friend class Log;

		std::list<formatter_t>	headers_;
		std::list<formatter_t>	tailers_;

		OStreamPtr	os_;

		filter_t	filter_;

	public:


		Gather(OStreamPtr os, filter_t filter = filter_t())
			: os_(os)
			, filter_(filter)
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

	protected:

		/// whether we should the record with the given level
		bool	output(log::level lvl)
		{
			return filter_ ? filter_(lvl) : true;
		}


		template <typename T>
		void log(T const & t)
		{
			*os_ << t;
		}


		virtual void on_record_begin(log::level lvl)
		{
			foreach(formatter_t & fmt, headers_)
			{
				fmt(*os_, lvl);
			}
		}

		virtual void on_record_end(log::level lvl)
		{
			foreach(formatter_t & fmt, tailers_)
			{
				fmt(*os_, lvl);
			}
		}
	};
}

#endif // LV_GATHER_HPP
