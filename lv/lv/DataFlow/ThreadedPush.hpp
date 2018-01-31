// *********************************************************************
//  ThreadedPush   version:  1.0   ¡¤  date: 10/26/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Concurrent/TaskQueue.hpp>
#include <lv/Concurrent/FIFOQueue.hpp>
#include <lv/Concurrent/ThreadGroup.hpp>

#include <lv/DataFlow/PushPolicyBase.hpp>

#include <functional>
#include <thread>


namespace lv::flow
{
	namespace detail
	{
		template<class T>
		class ThreadedPushImpl : public PushPolicyBase<T>
		{
			ThreadGroup threads_;

			TaskQueue<T, FIFOQueue> queue_;

		public:

			ThreadedPushImpl(size_t thread_num)
			{
				for (size_t i = 0; i < thread_num; ++i)
				{
					threads_.create_thread(std::bind(&ThreadedPushImpl::run, this));
				}
			}

			~ThreadedPushImpl()
			{
				queue_.close();
				threads_.join_all();
			}

			void	operator () (T const & t)
			{
				queue_.put(t);
			}

		private:

			void	run()
			{
				while (true)
				{
					try
					{
						this->callback_(queue_.get());
					}
					catch (TaskQueueClosed const &)
					{
						break;
					}
				}
			}

		};
	}

	template<class T>
	class ThreadedPush : public PushPolicyBase<T>
	{
		using Callback = typename PushPolicyBase<T>::Callback;

		// Make this class copyable
		std::shared_ptr<detail::ThreadedPushImpl<T> > impl_;

	public:

		ThreadedPush(size_t thread_num = 1)
			: impl_(std::make_shared<detail::ThreadedPushImpl<T> >(thread_num))
		{
		}

	
		void operator () (T const & t)
		{
			(*impl_)(t);
		}

		// not intended to be called by the user
		void	set_callback(Callback const & callback) override
		{
			impl_->set_callback(callback);
		}
	};

} 
