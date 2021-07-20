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

#include <lv/lvlib2.hpp>
#include <lv/Concurrent/Collector.hpp>

#include <boost/asio/io_context.hpp>


namespace lv::concurrent
{
	namespace detail
	{
		boost::asio::io_context &	io_context();
	}


	template<class Tasks, class Fn>
	decltype(auto)	fork_join(boost::asio::io_context & io, Tasks && tasks, Fn fn)
	{
		using result_type = std::decay_t<decltype(fn(*std::begin(tasks)))>;

		Collector<result_type> collector(std::size(tasks));
		size_t index = 0;

		for (auto && v : tasks)
		{
			if constexpr (std::is_rvalue_reference_v<decltype(v)>)
			{
				io.post(collector.wrap(index, [&fn, v = std::move(v)]() mutable {
					return fn(std::move(v));
				}));
			}
			else
			{
				io.post(collector.wrap(index, [&] { return fn(v); }));
			}

			index++;
		}

		return collector.get();
	}

	template<class Tasks, class Fn>
	decltype(auto)	fork_join(Tasks && tasks, Fn && fn)
	{
		return fork_join(detail::io_context(), std::forward<Tasks>(tasks), std::forward<Fn>(fn));
	}

}