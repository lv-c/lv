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
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <string>

namespace boost
{
	class thread_group;
}

namespace lv
{

	class ThreadPool
	{
	public:

		typedef boost::shared_ptr<boost::asio::io_service>		ServicePtr;

		typedef boost::function<void(std::string const &, std::exception const &)>	ExceptionHandler;

	private:

		typedef boost::weak_ptr<boost::asio::io_service>	WeakServicePtr;

		boost::scoped_ptr<boost::thread_group>	threads_;

		bool	thread_name_enabled_;

		ExceptionHandler	exception_handler_;

	public:

		// for security reason, you may not want to show the thread names in release version
		ThreadPool(bool enable_thread_name = true, ExceptionHandler const & handler = ExceptionHandler());

		// As long as the returned pointer is not released, the thread will
		// not stop. You should hold the pointer, or the thread will stop 
		// because no task has been posted to the service at the beginning.
		ServicePtr	create(std::string name = std::string(), size_t thread_num = 1);

		void	join();

	private:

		static	void	run(std::string name, WeakServicePtr weak_service, ExceptionHandler handler);
	};
}

#endif