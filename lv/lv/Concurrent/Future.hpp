// *********************************************************************
//  Future   version:  1.0   ¡¤  date: 2015/11/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <future>
#include <atomic>
#include <functional>


namespace lv
{
	template<class Ret> class Promise;

	template<class Ret>
	class Future : public std::shared_future<Ret>
	{
		using base_type = std::shared_future<Ret>;

		std::shared_ptr<Promise<Ret> >	promise_;

	private:

		template<class> friend class Promise;

		Future(std::shared_future<Ret> future, std::shared_ptr<Promise<Ret> > promise)
			: base_type(future)
			, promise_(promise)
		{
		}

	public:

		using value_type = Ret;

		/// creates an empty future
		Future() {}

		bool	is_ready() const
		{
			return promise_->is_ready();
		}

		// if it's ready now, the callback function will be called immediately, otherwise it will be called
		// from the thread where Promise::set_value or Promise::set_exception is called
		template<class T>
		void	set_callback(T && t) const
		{
			promise_->set_callback(std::forward<T>(t));
		}
	};

	using Acknowledgment = Future<void>;


	namespace detail
	{
		inline void	promise_set_value(std::promise<void> & p, void const *)
		{
			p.set_value();
		}

		template<class T>
		void	promise_set_value(std::promise<T> & p, T const * val)
		{
			p.set_value(*val);
		}
	}


	template<class Ret>
	class Promise : public std::enable_shared_from_this<Promise<Ret> >
	{
		std::promise<Ret>	promise_;

		// promise_.get_future can only be called once
		std::shared_future<Ret>	future_;

		// http://stackoverflow.com/questions/14365595/is-it-necessary-to-use-a-stdatomic-to-signal-that-a-thread-has-finished-execut
		std::atomic<bool>		is_ready_;

		using Callback = std::function<void(Future<Ret>)>;

		Callback	callback_;

		std::mutex	mutex_;

	public:

		Promise()
			: is_ready_(false)
		{
			future_ = promise_.get_future();
		}

		Future<Ret>	get_future()
		{
			return Future<Ret>(future_, this->shared_from_this());
		}

		// TODO : r-value is not supported

		void	set_value(Ret const * val)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			detail::promise_set_value(promise_, val);
			is_ready_ = true;

			signal();
		}

		void	set_exception(std::exception_ptr p)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			promise_.set_exception(p);
			is_ready_ = true;

			signal();
		}

	private:

		void	signal()
		{
			if (callback_)
			{
				callback_(get_future());
			}
		}

		template<class> friend class Future;

		bool	is_ready() const
		{
			return is_ready_;
		}

		void	set_callback(Callback cb)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			callback_ = std::move(cb);

			if (is_ready_)
			{
				signal();
			}
		}
	};

}
