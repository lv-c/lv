// *********************************************************************
//  Future   version:  1.0   ��  date: 2015/11/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_FUTURE_HPP
#define LV_CONCURRENT_FUTURE_HPP

#include <lv/LazyInit.hpp>

#include <boost/signals2/signal.hpp>

#include <future>


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

		Future(std::shared_future<Ret> future, std::shared_ptr<Promise<Ret> > promise)
			: base_type(future)
			, promise_(promise)
		{
		}

	public:

		typedef Ret	value_type;

		/// creates an empty future
		Future()
		{
		}

		// this callback function will be invoked within the same thread of Client::on_receive
		template<typename Slot>
		boost::signals2::connection	connect(Slot const & slot)
		{
			auto p = promise_.lock();

			if (p)
			{
				return p->connect(slot);
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

		typedef boost::signals2::signal<void(Future<Ret>)>	signal_type;

		LazeInit<signal_type>	signal_;

	public:

		Promise()
		{
			future_ = promise_.get_future();
		}

		Future<Ret>	get_future()
		{
			return Future<Ret>(future_, shared_from_this());
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
			if (signal_)
			{
				signal_.get() (get_future());
			}
		}

		template<typename> friend class Future;

		boost::signals2::connection	connect(typename signal_type::slot_type const & slot)
		{
			return signal_.get().connect(slot);
		}
	};

}

#endif