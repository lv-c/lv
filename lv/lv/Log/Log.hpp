// *********************************************************************
//  Log   version:  1.0   ��  date: 09/02/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LOG_HPP
#define LV_LOG_HPP

#include <iostream>

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include <lv/Log/Gather.hpp>


namespace lv
{
	/**
	 * Thread-safe
	 */
	class Log : boost::noncopyable
	{
		typedef boost::ptr_list<Gather>	gather_list;
		gather_list gathers_;

		typedef boost::mutex::scoped_lock	scoped_lock;	
		boost::mutex	mutex_;

	
		log::level	lvl_;	// level of the current record


		class Proxy
		{
			Log	&	log_;
		public:

			Proxy(Log & log)
				: log_(log)
			{
			}

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
		 * @code 
		 * log_(log::debug) << "hello world" << 4545;
		 * @endcode
		 */
		Proxy operator () (level lvl)
		{
			on_record_begin(lvl);

			return Proxy(*this);
		}

		void	add_gather(std::auto_ptr<Gather> gather)
		{
			scoped_lock lock(mutex_);

			this->gathers_.push_back(gather);
		}

	private:
		
		template <typename T>
		void log(T const & t)
		{
			foreach(Gather & gather, this->gathers_)
			{
				if(gather.log_level() <= this->lvl_)
					gather.log(t);
			}
		}

		void on_record_begin(level lvl)
		{
			// lock
			mutex_.lock();

			this->lvl_ = lvl;

			foreach(Gather & gather, this->gathers_)
			{
				if(gather.log_level() <= this->lvl_)
					gather.on_record_begin();
			}
		}

		void on_record_end()
		{
			foreach(Gather & gather, this->gathers_)
			{
				if(gather.log_level() <= this->lvl_)
					gather.on_record_end();
			}

			// unlock
			mutex_.unlock();
		}

	};
}

#endif // LV_LOG_HPP