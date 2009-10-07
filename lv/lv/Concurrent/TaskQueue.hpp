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

#ifndef LV_TASKQUEUE_HPP
#define LV_TASKQUEUE_HPP

#include <limits>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

/*
#include <lv/Concurrent/FIFOQueue.hpp>
#include <lv/Concurrent/PriorityQueue.hpp>
*/

namespace lv
{
	template <typename Task, template <typename> class QueuePolicy>
	class TaskQueue
	{
		QueuePolicy<Task>	queue_;


		typedef boost::mutex::scoped_lock	scoped_lock;
		mutable boost::mutex	mutex_;

		boost::condition	full_;
		boost::condition	empty_;
		boost::condition	consumer_interruption_done_;

		size_t				max_count_;		// maximum number of tasks
		size_t				consumers_to_interrupt_;
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
			scoped_lock lock(mutex_);

			while(queue_.size() >= max_count_)
			{
				full_.wait(lock);
			}

			if(queue_.empty())
				empty_.notify_all();

			queue_.push(task);
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
			scoped_lock lock(mutex_);

			while(true)
			{
				if(consumers_to_interrupt_ > 0)
				{
					consumers_to_interrupt_ --;

					if(consumers_to_interrupt_ == 0)
						consumer_interruption_done_.notify_all();

					throw boost::thread_interrupted();
				}

				if(! queue_.empty())
					break;

				empty_.wait(lock);
			}

			//
			if(queue_.size() == max_count_)
				full_.notify_all();
			//

			value_type task = queue_.top();
			queue_.pop();

			return task;
		}
		
		void	clear()
		{
			scoped_lock lock(mutex_);
			queue_.clear();
		}

		size_t	size() const
		{
			scoped_lock	lock(mutex_);
			return queue_.size();
		}

		bool	empty() const
		{
			scoped_lock lock(mutex_);
			return queue_.empty();
		}


		/**
		 * Interrupt num consumers that are currently waiting for tasks and that will call 'get' method later.
		 * The number of consumers to interrupt will be accumulated.
		 * @param num number of consumers to interrupt.
		 * @param wait wait for the job done or not
		 */
		void	interrupt_consumers(size_t num, bool wait = false)
		{
			scoped_lock	lock(mutex_);
			consumers_to_interrupt_ += num;
			
			empty_.notify_all();

			if(wait)
				consumer_interruption_done_.wait(lock);
		}

		/**
		 * Get the maximum number of tasks in the queue
		 */
		size_t	max_count() const
		{
			scoped_lock lock(mutex_);
			return max_count_;
		}

		/**
		 *  Set the maximum number of tasks in the queue
		 */
		void	max_count(size_t count)
		{
			scoped_lock lock(mutex_);
			max_count_ = count;
		}
	};


	/*
	// 
	template <typename Task>
	class FIFOTaskQueue : public TaskQueue<Task, FIFOQueue>
	{
	public:
		FIFOTaskQueue(size_t max_count = std::numeric_limits<size_t>::max())
			: TaskQueue(max_count)
		{
		}
	};

	template <typename Task>
	class PriorityTaskQueue : public TaskQueue<Task, PriorityQueue>
	{
	public:
		PriorityTaskQueue(size_t max_count = std::numeric_limits<size_t>::max())
			: TaskQueue(max_count)
		{
		}
	};
	*/
}


#endif // LV_TASKQUEUE_HPP