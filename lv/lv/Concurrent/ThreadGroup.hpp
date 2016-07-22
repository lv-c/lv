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

#include <memory>
#include <list>
#include <thread>
#include <shared_mutex>

// boost.thread_group has no timed join

namespace lv
{
	class ThreadGroup
	{
		typedef std::shared_ptr<std::thread>	ThreadPtr;

		std::list<ThreadPtr>	threads_;

		std::shared_timed_mutex		mutex_;

	public:

		template<typename F>
		std::thread *	create_thread(F func)
		{
			std::lock_guard<std::shared_timed_mutex> lock(mutex_);

			ThreadPtr new_thread = std::make_shared<std::thread>(func);
			threads_.push_back(new_thread);

			return new_thread.get();
		}

		size_t	size()
		{
			std::shared_lock<std::shared_timed_mutex> lock(mutex_);
			return threads_.size();
		}

		void	join_all()
		{
			std::shared_lock<std::shared_timed_mutex> lock(mutex_);

			for (ThreadPtr & v : threads_)
			{
				v->join();
			}
		}

		template<typename TimeDuration>
		inline bool	timed_join(TimeDuration const & rel_time)
		{
			boost::posix_time::ptime end_time = boost::get_system_time() + rel_time;

			std::shared_lock<std::shared_timed_mutex> lock(mutex_);

			for (ThreadPtr & v : threads_)
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