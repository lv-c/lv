// *********************************************************************
//  ThreadPool   version:  1.0   ¡¤  date: 08/07/2008
//  --------------------------------------------------------------------
//			Ïß³Ì³Ø
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_THREADPOOL_HPP
#define LV_THREADPOOL_HPP

#include <map>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/foreach.hpp>

#include <lv/Concurrent/TaskQueue.hpp>
#include <lv/Concurrent/FIFOQueue.hpp>

namespace lv
{
	template <
		typename Task, 
		template <typename> class SchedulingPolicy = FIFOQueue
	>
	class ThreadPool : private boost::noncopyable
	{
	public:
		// type definitions
		typedef Task	task_type;

		typedef TaskQueue<task_type, SchedulingPolicy>	taskqueue_type;

	private:

		taskqueue_type	task_queue_;

		typedef boost::mutex::scoped_lock	scoped_lock;
		boost::mutex	mutex_;
		// guarantee that at most one thread is executing 'resize' at one time
		boost::mutex	resize_mutex_;

		// threads which are terminated but might not have been fully destructed.
		std::list<boost::thread>	terminated_threads_;

		boost::condition	resize_done_;
		// 
		typedef boost::thread::id	thread_id;
		std::map<thread_id, boost::thread>	active_threads_;

		
		size_t	thread_num_;
		size_t	target_thread_num_;

	public:

		ThreadPool(size_t initial_threads = 0)	// throw(boost::thread_resource_error) (only if initial_threads > 0)
			: kill_all_threads_(false)
			, thread_num_(0)
			, target_thread_num_(0)
		{
			resize(initial_threads);
		}

		/**
		 * shutdown
		 * TODO : finish all the pending tasks before shutting down ? 
		 */
		~ThreadPool()
		{
			shutdown(false);
		}


		void	add_task(task_type const & task)
		{
			task_queue_.put(task);
		}

		/**
		 * Removes all the pending tasks.
		 */
		void	clear()
		{
			task_queue_.clear();
		}

		/**
		 * Retrieves the number of worker threads
		 */
		size_t	size()	const
		{
			scoped_lock lock(mutex_);
			return thread_num_;
		}

		/**
		 * Changes the number of worker threads
		 * @param wait wait for the threads terminated or not when the number of threads running is larger
		 *	than the target number.
		 */
		void	resize(size_t num, bool wait = false)	// throw(boost::thread_resource_error) (only if num > this->thread_num_)
		{
			scoped_lock resize_lock(resize_mutex_);

			scoped_lock lock(mutex_);

			target_thread_num_ = num;

			if(thread_num_ <= target_thread_num_)
			{
				while(thread_num_ < target_thread_num_)
				{
					boost::thread thread(boost::bind(ThreadPool::loop));
					active_threads_.insert(std::make_pair(thread.get_id(), thread));

					thread_num_ ++;
				}
			}
			else
			{
				task_queue_.interrupt_consumers(thread_num_ - target_thread_num_);

				// wait for the threads terminated
				if(wait)
				{
					resize_done_.wait(lock);

					BOOST_FOREACH(boost::thread & thread, terminated_threads_)
					{
						thread.join();
					}
					terminated_threads_.clear();
				}
			}
		}


		/**
		 * Terminates all the threads. Blocks until the job done.
		 * Note that this method will not try to execute all the pending tasks before shutting down.
		 * TODO : make it an atomic operation ? or finish all the pending tasks before shutting down ? 
		 */
		void	shutdown(bool clear_pending_tasks = false)
		{
			if(clear_pending_tasks)
				clear();

			resize(0, true);
		}

	private:
		void	loop()
		{
			while(true)
			{
				try
				{
					task_type task = task_queue_.get();

					// execute the task. TODO : handle exceptions ?
					task();
				}
				catch (boost::thread_interrupted const &)
				{
					scoped_lock lock(mutex_);
					if(thread_num_ > target_thread_num_)
					{
						thread_num_ --;
						if(thread_num_ == target_thread_num_)
							resize_done_.notify_all();

						// id of the current thread
						thread_id id = boost::this_thread::get_id();

						BOOST_ASSERT(active_threads_.find(id) != active_threads_.end());

						// terminates this thread
						terminated_threads_.push_back(active_threads_[id]);
						active_threads_.erase(id);

						break;
					}
				}
			}
		}

	};

}

#endif