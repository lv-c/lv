// *********************************************************************
//  TaskQueue   version:  1.0   ·  date: 08/07/2008
//  --------------------------------------------------------------------
//		Producer-Consumer 模式的任务列队， 线程安全
//		(Monitor Object Pattern 中的任务列队)
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Concurrent/Fwd.hpp>
#include <lv/Exception.hpp>
#include <lv/Ensure.hpp>

#include <limits>
#include <thread>
#include <condition_variable>


namespace lv
{
	DEFINE_EXCEPTION_MSG(TaskQueueClosed, std::runtime_error);


	template<class Task, template<class> class QueuePolicy>
	class TaskQueue
	{
		QueuePolicy<Task>	queue_;

		using lock_guard = std::lock_guard<std::mutex>;

		std::mutex	mutex_;

		std::condition_variable	full_;
		std::condition_variable	empty_;

		size_t		max_count_;		// maximum number of tasks
		bool		closed_;

	public:

		using value_type = Task;


		explicit TaskQueue(size_t max_count = std::numeric_limits<size_t>::max())
			: max_count_(max_count)
			, closed_(false)
		{
		}


		/** 
		 * Put a task into the queue. 
		 * If the queue is full, block until it's not full.
		 * @exception TaskQueueClosed if the task queue is closed
		 */
		void	put(value_type const & task)
		{
			put_impl(task);
		}

		void	put(value_type && task)
		{
			put_impl(std::forward<value_type>(task));
		}

		
		/**
		 * Get the next task that should be executed
		 * If the queue is empty, block until it's not empty.
		 * @exception TaskQueueClosed if the task queue is closed
		 */
		value_type get()
		{
			std::unique_lock<std::mutex> lock(mutex_);

			check_closed();

			while (queue_.empty())
			{
				empty_.wait(lock);
				check_closed();
			}

			//
			if (queue_.size() == max_count_)
			{
				full_.notify_all();
			}
			//

			value_type task = std::move(queue_.top());
			queue_.pop();

			return task;
		}
		
		void	clear()
		{
			lock_guard lock(mutex_);
			queue_.clear();
		}

		size_t	size()
		{
			lock_guard lock(mutex_);
			return queue_.size();
		}

		bool	empty()
		{
			lock_guard lock(mutex_);
			return queue_.empty();
		}


		void	close()
		{
			lock_guard lock(mutex_);

			closed_ = true;

			empty_.notify_all();
			full_.notify_all();
		}

		/**
		 * Get the maximum number of tasks in the queue
		 */
		size_t	max_count() const
		{
			return max_count_;
		}

		/**
		 *  Set the maximum number of tasks in the queue
		 */
		void	max_count(size_t count)
		{
			lock_guard lock(mutex_);
			max_count_ = count;
		}

	private:

		template<class T>
		void	put_impl(T && task)
		{
			std::unique_lock<std::mutex> lock(mutex_);

			check_closed();

			while (queue_.size() >= max_count_)
			{
				full_.wait(lock);
				check_closed();
			}

			if (queue_.empty())
			{
				empty_.notify_one();
			}

			queue_.push(std::forward<T>(task));
		}

		void	check_closed()
		{
			LV_ENSURE(!closed_, TaskQueueClosed());
		}

	};

}
