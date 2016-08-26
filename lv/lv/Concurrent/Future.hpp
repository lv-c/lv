// *********************************************************************
//  Future   version:  1.0   ¡¤  date: 2015/11/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_FUTURE_HPP
#define LV_CONCURRENT_FUTURE_HPP

#include <future>
#include <functional>


namespace lv
{
	template<typename Ret> class Promise;

	template<typename Ret>
	class Future : public std::shared_future<Ret>
	{
		typedef	std::shared_future<Ret>	base_type;

		std::weak_ptr<Promise<Ret> >	promise_;

	private:

		template<typename> friend class Promise;

		Future(std::shared_future<Ret> future, std::weak_ptr<Promise<Ret> > promise)
			: base_type(future)
			, promise_(promise)
		{
		}

	public:

		typedef Ret	value_type;

		/// creates an empty future
		Future() {}

		template<class T>
		void	set_callback(T && t) const
		{
			if (auto p = promise_.lock())
			{
				return p->set_callback(std::forward<T>(t));
			}
			else
			{
				throw std::future_error(std::make_error_code(std::future_errc::broken_promise));
			}
		}
	};

	typedef Future<void>	Acknowledgment;


	namespace detail
	{
		inline void	promise_set_value(std::promise<void> & p, void const *)
		{
			p.set_value();
		}

		template<typename T>
		void	promise_set_value(std::promise<T> & p, T const * val)
		{
			p.set_value(*val);
		}
	}


	template<typename Ret>
	class Promise : public std::enable_shared_from_this<Promise<Ret> >
	{
		std::promise<Ret>	promise_;

		// promise_.get_future can only be called once
		std::shared_future<Ret>	future_;

		typedef std::function<void(Future<Ret>)>	Callback;

		Callback	callback_;

		std::mutex	mutex_;

	public:

		Promise()
		{
			future_ = promise_.get_future();
		}

		Future<Ret>	get_future()
		{
			return Future<Ret>(future_, this->shared_from_this());
		}

		void	set_value(Ret const * val)
		{
			detail::promise_set_value(promise_, val);
			signal();
		}

		void set_exception(std::exception_ptr p)
		{
			promise_.set_exception(p);
			signal();
		}

	private:

		void	signal()
		{
			std::lock_guard<std::mutex> lock(mutex_);

			if (callback_)
			{
				callback_(get_future());
			}
		}

		template<typename> friend class Future;

		void	set_callback(Callback cb)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			callback_ = std::move(cb);
		}
	};

}

#endif
