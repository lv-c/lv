// *********************************************************************
//  PriorityQueue   version:  1.0   ·  date: 08/07/2008
//  --------------------------------------------------------------------
//	优先列队。	TaskQueue 中 QueuePolicy 的一个实现
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <queue>

namespace lv
{
	template<class T>
	class PriorityQueue
	{
		std::priority_queue<T>	container_;

	public:

		using value_type = T;


		void	push(value_type const & value)
		{
			container_.push(value);
		}

		void	push(value_type && value)
		{
			container_.push(std::forward<value_type>(value));
		}

		void	pop()
		{
			container_.pop();
		}

		/** 
		 * Get a reference to the next object (that should be executed)
		 */
		value_type &	top()
		{
			return container_.top();
		}

		void	clear()
		{
			container_ = std::priority_queue<T>();
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
