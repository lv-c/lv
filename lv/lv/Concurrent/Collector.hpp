// *********************************************************************
//  Collector   version:  1.0   ¡¤  date: 2017/08/31
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2017 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/noncopyable.hpp>

#include <vector>
#include <atomic>
#include <future>
#include <type_traits>


namespace lv
{

	// returning when the first exception is thrown is error prone, so we wait for all the results

	template<class R>
	class Collector : boost::noncopyable
	{
		std::atomic<size_t>			counter_;

		using is_void_result = std::is_same<R, void>;
		using results_type = std::conditional_t<is_void_result::value, void, std::vector<R> >;

		struct NullType { NullType(...) {} };

		std::conditional_t<is_void_result::value, NullType, results_type>	results_;

		std::exception_ptr			first_exception_;

		std::promise<results_type>	promise_;

		std::future<results_type>	future_;

		// writing to different locations of a vector is not guaranteed to be 
		// thread-safe by the standard
		// http://stackoverflow.com/questions/6455144/is-vector-threadsafe-for-read-write-at-different-locations
		// (and it's certainly not thread-safe if T is bool)
		std::mutex					mutex_;


		// we don't use std::function here to do the type erasure because it requires 
		// Fn to be copyable and copy constructible

		class FnWrapperBase
		{
		public:

			virtual	R	exec() = 0;
		};

		template<class Fn>
		class FnWrapper : public FnWrapperBase
		{
			size_t		index_;

			Fn			fn_;

			Collector &	collector_;

		public:

			FnWrapper(size_t index, Fn fn, Collector & collector)
				: index_(index)
				, fn_(std::move(fn))
				, collector_(collector)
			{
			}

			void operator () ()
			{
				collector_.invoke(index_, *this);
			}

			R	exec() override
			{
				return fn_();
			}
		};

	public:

		explicit Collector(size_t size)
			: counter_(size)
			, results_(size)
			, future_(promise_.get_future())
		{
			if (size == 0)
			{
				set_value(is_void_result());
			}
		}

		~Collector()
		{
			// wait for other threads to release the mutex
			std::lock_guard<std::mutex> lock(mutex_);
		}

		template<class Fn>
		FnWrapper<Fn>	wrap(size_t index, Fn && fn)
		{
			return FnWrapper<Fn>(index, std::forward<Fn>(fn), *this);
		}

		results_type	get()
		{
			return future_.get();
		}

	private:

		void	invoke(size_t index, FnWrapperBase & fn_wrapper)
		{
			try
			{
				invoke_impl(index, fn_wrapper, is_void_result());
			}
			catch (...)
			{
				std::lock_guard<std::mutex> lock(mutex_);

				if (!first_exception_)
				{
					first_exception_ = std::current_exception();
				}
			}

			if (--counter_ == 0)
			{
				std::lock_guard<std::mutex> lock(mutex_);

				if (first_exception_)
				{
					promise_.set_exception(first_exception_);
				}
				else
				{
					set_value(is_void_result());
				}
			}
		}

		void	invoke_impl(size_t index, FnWrapperBase & fn_wrapper, std::true_type /* is_void_result */)
		{
			fn_wrapper.exec();
		}

		void	invoke_impl(size_t index, FnWrapperBase & fn_wrapper, std::false_type /* is_void_result */)
		{
			R ret = fn_wrapper.exec();

			//
			std::lock_guard<std::mutex> lock(mutex_);

			results_.at(index) = std::move(ret);
		}

		void	set_value(std::true_type /* is_void_result */)
		{
			promise_.set_value();
		}

		void	set_value(std::false_type /* is_void_result */)
		{
			promise_.set_value(std::move(results_));
		}
	};

}
