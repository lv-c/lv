// *********************************************************************
//  FIFOQueue   version:  1.0   ·  date: 08/07/2008
//  --------------------------------------------------------------------
//		TaskQueue 中 QueuePolicy 的一个先进先出的实现
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <deque>

namespace lv
{
	template<class T>
	class FIFOQueue
	{
		std::deque<T>	container_;

	public:

		using value_type = T;

		
		void	push(value_type const & value)
		{
			container_.push_back(value);
		}

		void	push(value_type && value)
		{
			container_.push_back(std::forward<value_type>(value));
		}

		void	pop()
		{
			container_.pop_front();
		}

		/** 
		 * Get a reference to the next object (that should be executed)
		 */
		value_type &	top()
		{
			return container_.front();
		}

		void	clear()
		{
			container_.clear();
		}


		size_t	size() const
		{
			return container_.size();
		}

		bool	empty() const
		{
			return container_.empty();
		}
	};
}