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

#include <list>
#include <iostream>

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <lv/Log/Gather.hpp>
#include <lv/Log/Formatter.hpp>


namespace lv { namespace log {

	/**
	 * Thread-safe
	 */
	class Log : boost::noncopyable
	{
		typedef std::list<gather_ptr>	gather_list;
		gather_list gathers_;

		typedef boost::mutex::scoped_lock	scoped_lock;	
		boost::mutex	mutex_;

	
		level	lvl_;	// level of the current record


		class Proxy
		{
			Log	&	log_;
		public:

			Proxy(Log & log) : log_(log) {}

			~Proxy()
			{
				log_.on_record_end();
			}

			template <typename T>
			Proxy & operator << (T const & t)
			{
				log_.log(t);
				return *this;
			}
		};

	public:

		/**
		 * the default parameter will make your life easier if you just want a simple
		 *	logger
		 * @code 
		 * log_(debug) << "hello world" << 4545;
		 * @endcode
		 */
		Proxy operator () (level lvl = info)
		{
			on_record_begin(lvl);

			return Proxy(*this);
		}

		void	add_gather(gather_ptr gather)
		{
			scoped_lock lock(mutex_);

			this->gathers_.push_back(gather);
		}

	private:
		
		template <typename T>
		void log(T const & t)
		{
			foreach(gather_ptr & gather, this->gathers_)
			{
				if(gather->output(lvl_))
					gather->log(t);
			}
		}

		void on_record_begin(level lvl)
		{
			// lock
			mutex_.lock();

			this->lvl_ = lvl;

			foreach(gather_ptr & gather, this->gathers_)
			{
				if(gather->output(lvl_))
					gather->on_record_begin(lvl_);
			}
		}

		void on_record_end()
		{
			foreach(gather_ptr & gather, this->gathers_)
			{
				if(gather->output(lvl_))
					gather->on_record_end(lvl_);
			}

			// unlock
			mutex_.unlock();
		}

	};
} }

#endif // LV_LOG_HPP