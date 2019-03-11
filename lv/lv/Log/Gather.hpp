// *********************************************************************
//  Gather   version:  1.0   ¡¤  date: 09/02/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Log/Fwd.hpp>
#include <lv/Log/Level.hpp>
#include <lv/ScopeExit.hpp>

#include <iostream>
#include <list>
#include <memory>
#include <mutex>


namespace lv::log
{
	// filters
	
	template<class Pred>
	class UnaryFilter
	{
		int		lvl_;

		Pred	pred_;

	public:

		UnaryFilter(int lvl, Pred pred = Pred()) 
			: lvl_(lvl)
			, pred_(pred)
		{
		}
		
		bool operator () (int l) const
		{ 
			return pred_(l, lvl_); 
		}
	};

	using EqualFilter = UnaryFilter<std::equal_to<int> >;
	using GreaterEqualFilter = UnaryFilter<std::greater_equal<int> >;
	using LessEualFilter = UnaryFilter<std::less_equal<int> >;

	// do not output debug messages in release version
	struct DebugFilter
	{
		bool operator () (int l) const
		{
#ifdef _DEBUG
			return true;
#else
			return l > debug;
#endif
		}
	};

	/// A gather could be shared among different loggers.
	class Gather
	{
	protected:

		friend class Log;

		std::list<formatter_type>	headers_;
		std::list<formatter_type>	tailers_;

		ostream_ptr	os_;

		filter_type	filter_;

		std::mutex	mutex_;

	public:


		Gather(ostream_ptr os, filter_type filter = filter_type())
			: os_(os)
			, filter_(filter)
		{
		}

		/// The following three functions are not thread-safe. Usually you should
		/// only call them at the very beginning

		void	set_filter(filter_type filter)
		{
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

		bool	filter(int lvl)
		{
			return filter_ ? filter_(lvl) : true;
		}


		template<class T>
		void	log(T const & t)
		{
			*os_ << t;
		}

#ifdef LV_UNICODE_LOG

		void	log(std::string const & t)
		{
			std::copy(t.begin(), t.end(), std::ostream_iterator<char, char_type>(*os_));
		}

#endif


		virtual void	on_record_begin(int lvl)
		{
			// lock
			mutex_.lock();

			for (formatter_type & fmt : headers_)
			{
				fmt(*os_, lvl);
			}
		}

		/// overloaded function should take care of the unlock
		virtual void	on_record_end(int lvl)
		{
			LV_SCOPE_EXIT([this] { mutex_.unlock(); });

			end_record(lvl);
		}

		void	end_record(int lvl)
		{
			for (formatter_type & fmt : tailers_)
			{
				fmt(*os_, lvl);
			}
		}
	};
}
