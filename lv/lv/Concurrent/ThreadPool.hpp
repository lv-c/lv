// *********************************************************************
//  ThreadPool   version:  1.0   ¡¤  date: 2017/08/31
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2017 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_THREADPOOL_HPP
#define LV_CONCURRENT_THREADPOOL_HPP

#include <vector>
#include <thread>
#include <memory>

#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>


namespace lv
{
	class ThreadPool : boost::noncopyable
	{
		std::vector<std::thread>	threads_;

		boost::asio::io_service		service_;

		typedef boost::asio::io_service::work	work_type;

		std::unique_ptr<work_type>	work_;

	public:

		ThreadPool(unsigned thread_count = std::thread::hardware_concurrency());

		~ThreadPool();

		boost::asio::io_service &	service();

		size_t	size() const;

	};
}

#endif