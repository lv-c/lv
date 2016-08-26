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
#include <mutex>


namespace lv
{
	class ThreadGroup
	{
		typedef std::unique_ptr<std::thread>	ThreadPtr;

		std::list<ThreadPtr>	threads_;

		std::mutex		mutex_;

	public:

		template<typename F>
		std::thread &	create_thread(F && func)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			ThreadPtr new_thread = std::make_unique<std::thread>(std::forward<F>(func));
			threads_.push_back(std::move(new_thread));

			return *(threads_.back());
		}

		size_t	size()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return threads_.size();
		}

		void	join_all()
		{
			std::lock_guard<std::mutex> lock(mutex_);

			for (ThreadPtr & v : threads_)
			{
				v->join();
			}
		}
	};
}

#endif
