// *********************************************************************
//  Log   version:  1.0   ¡¤  date: 09/02/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LOG_HPP
#define LV_LOG_HPP

#include <lv/Log/Gather.hpp>
#include <lv/Log/Formatter.hpp>

#include <boost/noncopyable.hpp>

#include <set>
#include <iostream>
#include <memory>
#include <mutex>


namespace lv { namespace log {

	/**
	 * Thread-safe
	 */
	class Log : boost::noncopyable
	{
		// we use set rather than list here to prevent deadlock when some
		// gathers are shared among different loggers
		typedef std::set<gather_ptr>	gather_set;
		gather_set gathers_;

		typedef std::lock_guard<std::mutex>	lock_guard;

		std::mutex	mutex_;

		volatile bool	enabled_;

	
		int		lvl_;	// level of the current record


		class Proxy : boost::noncopyable
		{
			Log	&	log_;

			bool	active_;

		public:

			Proxy(Log & log) 
				: log_(log)
				, active_(true)
			{
			}

			Proxy(Proxy && rhs)
				: log_(rhs.log_)
				, active_(true)
			{
				rhs.active_ = false;
			}

			~Proxy()
			{
				if (active_)
				{
					log_.on_record_end();
				}
			}

			template <typename T>
			Proxy & operator << (T const & t)
			{
				BOOST_ASSERT(active_);

				log_.log(t);
				return *this;
			}

			// streaming std::endl, std::ends, std::flush ...
			Proxy & operator << (ostream_type & (* fun)(ostream_type &))
			{
				BOOST_ASSERT(active_);

				log_.log(fun);
				return *this;
			}
		};

	public:

		Log()
			: enabled_(true)
		{
		}

		/**
		 * the default parameter will make your life easier if you just want a simple
		 *	logger
		 * @code 
		 * log_(debug) << "hello world" << 4545;
		 * @endcode
		 */
		Proxy operator () (int lvl = info)
		{
			on_record_begin(lvl);

			return Proxy(*this);
		}

		void	add_gather(gather_ptr gather)
		{
			lock_guard lock(mutex_);

			this->gathers_.insert(gather);
		}

		void	remove_gather(gather_ptr gather)
		{
			lock_guard lock(mutex_);

			gathers_.erase(gather);
		}
		
		void	enable(bool enabled = true)
		{
			enabled_ = enabled;
		}

	private:
		
		template <typename T>
		void log(T const & t)
		{
			if (enabled_)
			{
				for (gather_ptr gather : this->gathers_)
				{
					if (gather->filter(lvl_))
					{
						gather->log(t);
					}
				}
			}
		}

		void on_record_begin(int lvl)
		{
			// lock
			mutex_.lock();

			this->lvl_ = lvl;

			if (enabled_)
			{
				for (gather_ptr gather : this->gathers_)
				{
					if (gather->filter(lvl_))
					{
						gather->on_record_begin(lvl_);
					}
				}
			}
		}

		void on_record_end()
		{
			LV_SCOPE_EXIT([this] { mutex_.unlock(); });

			if (enabled_)
			{
				for (gather_ptr gather : this->gathers_)
				{
					if (gather->filter(lvl_))
					{
						gather->on_record_end(lvl_);
					}
				}
			}
		}

	};
} }

#endif
