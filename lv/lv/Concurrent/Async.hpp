// *********************************************************************
//  Async   version:  1.0   ¡¤  date: 2017/08/31
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2017 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Concurrent/Collector.hpp>

#include <boost/asio/io_service.hpp>


namespace lv::concurrent
{
	namespace detail
	{
		boost::asio::io_service &	get_service();
	}


	template<class Tasks, class Fn>
	decltype(auto)	fork_join(boost::asio::io_service & service, Tasks & tasks, Fn fn)
	{
		using result_type = decltype(fn(*std::begin(tasks)));

		Collector<result_type> collector(std::size(tasks));
		size_t index = 0;

		for (auto & v : tasks)
		{
			service.post(collector.wrap(index, [&] {
				return fn(v);
			}));

			index++;
		}

		return collector.get();
	}

	template<class Tasks, class Fn>
	decltype(auto)	fork_join(Tasks && tasks, Fn && fn)
	{
		return fork_join(detail::get_service(), std::forward<Tasks>(tasks), std::forward<Fn>(fn));
	}

}