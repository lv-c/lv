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

#include <iostream>
#include <list>

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include <lv/Log/Gather.hpp>


namespace lv
{

	namespace detail
	{
		template <class Log>
		class LogProxy
		{
			Log	&	log_;
		public:

			LogProxy(Log & log)
				: log_(log)
			{
			}

			~LogProxy()
			{
				log_.on_record_end();
			}

			template <typename T>
			LogProxy & operator << (T const & t)
			{
				log_.log(t);
				return *this;
			}
		};
	}


	class Log
	{

		typedef detail::LogProxy<Log>	proxy_t;
		friend class proxy_t;

		typedef boost::shared_ptr<Gather> gather_ptr;
		std::list<gather_ptr>	gathers_;

		boost::mutex	mutex_;

		bool const	syn_;
	
		log::level	level_;	// level of the current record

	public:

		/**
		 * @param syn synchronize logging operations or not. Set it true if this logger
		 *	is intended to be used in multiple threads.
		 */
		Log(bool syn = true)
			: syn_(syn)
		{
		}


		/**
		 * @code 
		 * log_(log::debug) << "hello world" << 4545;
		 * @endcode
		 */
		proxy_t operator () (level lvl)
		{
			on_record_begin(lvl);

			return proxy_t(*this);
		}

		void	add_gather(gather_ptr gather)
		{
			this->gathers_.push_back(gather);
		}

	private:
		
		template <typename T>
		void log(T const & t)
		{
			foreach(gather_ptr & gather, this->gathers_)
			{
				if(gather->log_level() <= this->lvl_)
					gather->log(t);
			}
		}

		void on_record_begin(level lvl)
		{
			if(syn_)
				mutex_.lock();

			this->lvl_ = lvl;

			foreach(gather_ptr & gather, this->gathers_)
			{
				if(gather->log_level() <= this->lvl_)
					gather->on_record_begin();
			}
		}

		void on_record_end()
		{
			foreach(gather_ptr & gather, this->gathers_)
			{
				if(gather->log_level() <= this->lvl_)
					gather->on_record_end();
			}

			if(syn_)
				mutex_.unlock();
		}

	};
}

#endif // LV_LOG_HPP