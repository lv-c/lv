// *********************************************************************
//  ThreadPool   version:  1.0   ·  date: 08/07/2008
//  --------------------------------------------------------------------
//			线程池
//	有了 boost::asio::io_service, 之前的意义已经不大。
//	2013.1.24 重新实现了一份
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_THREADPOOL_HPP
#define LV_CONCURRENT_THREADPOOL_HPP

#include <lv/Concurrent/Fwd.hpp>

#include <boost/asio/io_service.hpp>

#include <string>
#include <memory>
#include <functional>


namespace lv
{
	class ThreadPool
	{
	public:

		typedef std::shared_ptr<boost::asio::io_service>		ServicePtr;

		typedef std::function<void(std::string const &, std::exception const &)>	ExceptionHandler;

	private:

		typedef std::weak_ptr<boost::asio::io_service>	WeakServicePtr;

		std::shared_ptr<ThreadGroup>	threads_;

		bool	thread_name_enabled_;

		ExceptionHandler	exception_handler_;

	public:

		// for ( security reason, you may not want to show the thread names in release version
		ThreadPool(ExceptionHandler const & handler = ExceptionHandler(), bool enable_thread_name = true);

		// As long as the returned pointer is not released, the thread will
		// not stop. You should hold the pointer, or the thread will stop 
		// because no task has been posted to the service at the beginning.
		ServicePtr	create(size_t thread_num = 1, std::string name = std::string());

		void	join_all();

		template<typename TimeDuration>
		inline bool	timed_join(TimeDuration const & rel_time)
		{
			return threads_->timed_join(rel_time);
		}

	private:

		static	void	run(std::string name, WeakServicePtr weak_service, ExceptionHandler handler);
	};
}

#endif