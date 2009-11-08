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

	
		int	lvl_;	// int of the current record


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

			Proxy(Proxy & rhs)
				: log_(rhs.log_)
				, active_(true)
			{
				rhs.active_ = false;
			}

			~Proxy()
			{
				if(active_)
					log_.on_record_end();
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
			scoped_lock lock(mutex_);

			this->gathers_.push_back(gather);
		}

		void	remove_gather(gather_ptr gather)
		{
			scoped_lock lock(mutex_);

			gather_list::iterator it = std::find(this->gathers_.begin(), this->gathers_.end(), gather);
			if(it != this->gathers_.end())
				this->gathers_.erase(it);
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

		void on_record_begin(int lvl)
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
