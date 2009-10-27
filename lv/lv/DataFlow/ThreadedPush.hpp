// *********************************************************************
//  ThreadedPush   version:  1.0   ��  date: 10/26/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_THREADEDPUSH_HPP
#define LV_DATAFLOW_THREADEDPUSH_HPP

#include <lv/Concurrent/TaskQueue.hpp>
#include <lv/Concurrent/FIFOQueue.hpp>

#include <lv/DataFlow/PushPolicyBase.hpp>

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

namespace lv { namespace flow {

	namespace detail
	{
		template<class T>
		class ThreadedPushImpl : public PushPolicyBase<T>
		{
			boost::thread_group threads_;

			TaskQueue<T, FIFOQueue> queue_;

		public:

			ThreadedPushImpl(size_t thread_num)
			{
				for(size_t i = 0; i < thread_num; ++i)
				{
					threads_.create_thread(boost::bind(&ThreadedPushImpl::run, this));
				}
			}

			~ThreadedPushImpl()
			{
				queue_.interrupt_consumers(threads_.size());

				threads_.join_all();
			}

			void	operator () (T const & t)
			{
				queue_.put(t);
			}

		private:

			void	run()
			{
				while(true)
				{
					try
					{
						callback_(queue_.get());
					}
					catch (boost::thread_interrupted const &)
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
		// Make this class copyable
		boost::shared_ptr<detail::ThreadedPushImpl<T> > impl_;

	public:

		explicit ThreadedPush(size_t thread_num = 1)
			: impl_(new detail::ThreadedPushImpl<T>(thread_num))
		{
		}

	
		void operator () (T const & t)
		{
			(*impl_)(t);
		}

		// not intended to be called by the user
		virtual	void	set_callback(Callback const & callback)
		{
			impl_->set_callback(callback);
		}
	};

} }

#endif 