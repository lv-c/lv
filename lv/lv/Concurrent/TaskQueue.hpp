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

#ifndef LV_CONCURRENT_TASKQUEUE_HPP
#define LV_CONCURRENT_TASKQUEUE_HPP

#include <lv/Concurrent/Fwd.hpp>
#include <lv/Exception.hpp>

#include <limits>
#include <thread>
#include <condition_variable>


namespace lv
{
	DEFINE_EXCEPTION_MSG(ThreadInterrupted, std::runtime_error);


	template <typename Task, template <typename> class QueuePolicy>
	class TaskQueue
	{
		QueuePolicy<Task>	queue_;

		typedef std::lock_guard<std::mutex>	lock_guard;

		std::mutex	mutex_;

		std::condition_variable	full_;
		std::condition_variable	empty_;
		std::condition_variable	consumer_interruption_done_;

		volatile size_t		max_count_;		// maximum number of tasks
		volatile size_t		consumers_to_interrupt_;

	public:

		typedef Task	value_type;


		TaskQueue(size_t max_count = std::numeric_limits<size_t>::max())
			: max_count_(max_count)
			, consumers_to_interrupt_(0)
		{
		}


		/** 
		 * Put a task into the queue. 
		 * If the queue is full, block until it's not full.
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
		 * Note that this function can be interrupted, which will throw a boost::thread_interrupted
		 * exception.
		 * @see interrupt_consumers
		 */
		value_type get() // throw(boost::thread_interrupted)
		{
			std::unique_lock<std::mutex> lock(mutex_);

			while (true)
			{
				if (consumers_to_interrupt_ > 0)
				{
					consumers_to_interrupt_ --;

					if (consumers_to_interrupt_ == 0)
					{
						consumer_interruption_done_.notify_all();
					}

					throw ThreadInterrupted();
				}

				if (!queue_.empty())
				{
					break;
				}

				empty_.wait(lock);
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


		/**
		 * Interrupt num consumers that are currently waiting for ( tasks and that will call 'get' method later.
		 * The number of consumers to interrupt will be accumulated.
		 * @param num number of consumers to interrupt.
		 * @param wait wait for ( the job done or not
		 */
		void	interrupt_consumers(size_t num, bool wait = false)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			consumers_to_interrupt_ += num;
			
			empty_.notify_all();

			if (wait)
			{
				consumer_interruption_done_.wait(lock);
			}
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
			max_count_ = count;
		}

	private:

		template<typename T>
		void	put_impl(T && task)
		{
			std::unique_lock<std::mutex> lock(mutex_);

			while (queue_.size() >= max_count_)
			{
				full_.wait(lock);
			}

			if (queue_.empty())
			{
				empty_.notify_one();
			}

			queue_.push(std::forward<T>(task));
		}

	};

}


#endif