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

#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>


namespace lv
{
	class ThreadPool : boost::noncopyable
	{
		std::vector<std::thread>	threads_;

		boost::asio::io_service		service_;

		using work_type = boost::asio::io_service::work;

		std::unique_ptr<work_type>	work_;

	public:

		ThreadPool(unsigned thread_count = std::thread::hardware_concurrency());

		~ThreadPool();

		boost::asio::io_service &	service();

		size_t	size() const;

	};
}