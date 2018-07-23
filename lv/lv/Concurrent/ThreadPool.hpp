// *********************************************************************
//  ThreadPool   version:  1.0   ¡¤  date: 2017/08/31
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2017 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <vector>
#include <thread>
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/noncopyable.hpp>


namespace lv
{
	class ThreadPool : boost::noncopyable
	{
		std::vector<std::thread>	threads_;

		boost::asio::io_context		io_;

		using work_gurad = boost::asio::executor_work_guard<
			boost::asio::io_context::executor_type>;

		work_gurad	work_;

	public:

		ThreadPool(unsigned thread_count = std::thread::hardware_concurrency());

		~ThreadPool();

		boost::asio::io_context &	io_context();

		size_t	size() const;

	};
}