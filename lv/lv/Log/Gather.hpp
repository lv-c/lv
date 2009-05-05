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
#include <boost/thread/mutex.hpp>

#include <lv/Log/Fwd.hpp>
#include <lv/Log/Level.hpp>
#include <lv/Foreach.hpp>

namespace lv { namespace log {

	// filters
	
	template<class Pred>
	class UnaryFilter
	{
		Pred	pred_;

		level	lvl_;
	public:
		UnaryFilter(level lvl, Pred pred = Pred()) 
			: lvl_(lvl)
			, pred_(pred)
		{
		}
		
		bool operator () (level l) const
		{ 
			return pred_(l, lvl_); 
		}
	};

	typedef UnaryFilter<std::equal_to<level> >			EqualFilter;
	typedef UnaryFilter<std::greater_equal<level> >	GreaterEqualFilter;
	typedef UnaryFilter<std::less_equal<level> >		LessEualFilter;

	// do not output debug messages in release version
	struct DebugFilter
	{
		bool operator () (level l) const
		{
#ifdef _DEBUG
			return true;
#else
			return l > debug;
#endif
		}
	};

	class Gather
	{
	protected:

		friend class Log;

		std::list<formatter_type>	headers_;
		std::list<formatter_type>	tailers_;

		ostream_ptr	os_;

		filter_type	filter_;

		typedef boost::mutex::scoped_lock	scoped_lock;
		boost::mutex	mutex_;

	public:


		Gather(ostream_ptr os, filter_type filter = filter_type())
			: os_(os)
			, filter_(filter)
		{
		}

		void	set_filter(filter_type filter)
		{
			scoped_lock lock(mutex_);

			this->filter_ = filter;
		}

		
		Gather &	add_header(formatter_type header)
		{
			this->headers_.push_back(header);
			return *this;
		}

		Gather &	add_tailer(formatter_type tailer)
		{
			this->tailers_.push_back(tailer);
			return *this;
		}

	protected:

		/// whether we should output the record with the given level
		bool	output(level lvl)
		{
			scoped_lock lock(mutex_);

			return filter_ ? filter_(lvl) : true;
		}


		template <typename T>
		void log(T const & t)
		{
			*os_ << t;
		}


		virtual void on_record_begin(level lvl)
		{
			foreach(formatter_type & fmt, headers_)
			{
				fmt(*os_, lvl);
			}
		}

		virtual void on_record_end(level lvl)
		{
			foreach(formatter_type & fmt, tailers_)
			{
				fmt(*os_, lvl);
			}
		}
	};
} }

#endif // LV_GATHER_HPP
