// *********************************************************************
//  Log   version:  1.0   ¡¤  date: 09/02/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Log/Gather.hpp>
#include <lv/Log/Formatter.hpp>

#include <boost/noncopyable.hpp>

#include <set>
#include <iostream>
#include <memory>
#include <mutex>
#include <exception>


namespace lv::log
{
	/**
	 * Thread-safe
	 */
	class Log : boost::noncopyable
	{
		// we use set rather than list here to prevent deadlock when some
		// gathers are shared among different loggers
		using gather_set = std::set<gather_ptr>;
		gather_set gathers_;

		using lock_guard = std::lock_guard<std::mutex>;

		std::mutex	mutex_;

		volatile bool	enabled_;

	
		int		lvl_;	// level of the current record


		class Proxy : boost::noncopyable
		{
			Log	*	log_;

		public:

			Proxy(Log & log) 
				: log_(&log)
			{
			}

			Proxy(Proxy && rhs)
				: log_(rhs.log_)
			{
				rhs.log_ = nullptr;
			}

			~Proxy() noexcept(false)
			{
				if (log_ != nullptr)
				{
					log_->on_record_end();
				}
			}

			template<class T>
			Proxy &	operator << (T && t)
			{
				log_->log(std::forward<T>(t));
				return *this;
			}

			// streaming std::endl, std::ends, std::flush ...
			Proxy &	operator << (ostream_type & (* fn)(ostream_type &))
			{
				log_->log(fn);
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
		Proxy	operator () (int lvl = info)
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
			// locking is necessary to avoid setting @enabled_ when a record is being logged
			lock_guard lock(mutex_);

			enabled_ = enabled;
		}

	private:
		
		template<class T>
		void	log(T const & t)
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

		void	on_record_begin(int lvl)
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


		// there may be circumstances that users want to throw an exception (to terminate the execution)
		// when an error message is received (inside @Gather::on_record_end). only the last exception will
		// be rethrown if multiple exceptions are caught
		void	on_record_end()
		{
			LV_SCOPE_EXIT([this] { mutex_.unlock(); });

			if (enabled_)
			{
				std::exception_ptr last_ex;

				for (gather_ptr gather : this->gathers_)
				{
					if (gather->filter(lvl_))
					{
						try
						{
							gather->on_record_end(lvl_);
						}
						catch (...)
						{
							last_ex = std::current_exception();
						}
					}
				}

				if (last_ex)
				{
					std::rethrow_exception(last_ex);
				}
			}
		}

	};
}
