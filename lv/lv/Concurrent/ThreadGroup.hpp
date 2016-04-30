// *********************************************************************
//  ThreadGroup   version:  1.0   ¡¤  date: 2013/03/03
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_THREADGROUP_HPP
#define LV_CONCURRENT_THREADGROUP_HPP

#include <lv/Foreach.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <memory>
#include <list>

// boost.thread_group has no timed join

namespace lv
{
	class ThreadGroup
	{
		typedef std::shared_ptr<boost::thread>	ThreadPtr;

		std::list<ThreadPtr>	threads_;

		boost::shared_mutex		mutex_;

	public:

		template<typename F>
		boost::thread *	create_thread(F func)
		{
			boost::lock_guard<boost::shared_mutex> lock(mutex_);

			ThreadPtr new_thread(new boost::thread(func));
			threads_.push_back(new_thread);

			return new_thread.get();
		}

		void	join_all()
		{
			boost::shared_lock<boost::shared_mutex> lock(mutex_);

			BOOST_FOREACH(ThreadPtr v, threads_)
			{
				v->join();
			}
		}

		template<typename TimeDuration>
		inline bool	timed_join(TimeDuration const & rel_time)
		{
			boost::posix_time::ptime end_time = boost::get_system_time() + rel_time;

			boost::shared_lock<boost::shared_mutex> lock(mutex_);

			BOOST_FOREACH(ThreadPtr v, threads_)
			{
				if (! v->timed_join(end_time))
				{
					return false;
				}
			}

			return true;
		}
	};
}

#endif