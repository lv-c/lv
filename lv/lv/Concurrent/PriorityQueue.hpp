// *********************************************************************
//  PriorityQueue   version:  1.0   ·  date: 08/07/2008
//  --------------------------------------------------------------------
//	优先列队。	TaskQueue 中 QueuePolicy 的一个实现
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_PRIORITYQUEUE_HPP
#define LV_CONCURRENT_PRIORITYQUEUE_HPP

#include <queue>

namespace lv
{
	template <typename T>
	class PriorityQueue
	{
		std::priority_queue<T>	container_;
	public:
		typedef T value_type;


		
		void	push(value_type const & value)
		{
			container_.push(value);
		}

		void	push(value_type && value)
		{
			container_.push(value);
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


#endif