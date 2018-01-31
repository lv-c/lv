// *********************************************************************
//  AsyncFileIO   version:  1.0   ·  date: 08/09/2008
//  --------------------------------------------------------------------
//		为同步 IO 提供异步 IO 的能力
//		Proxy Pattern
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FileSystem/Fwd.hpp>
#include <lv/FileSystem/IFileIO.hpp>
#include <lv/ServiceWrapper.hpp>

#include <future>


namespace lv
{
	class AsyncFileIO : public IFileIO
	{
		IFileIOPtr	sync_io_;

		ServiceWrapper	service_wrapper_;

	public:
		
		AsyncFileIO(IFileIOPtr synio, ServiceWrapper const & service_wrapper)
			: sync_io_(synio)
			, service_wrapper_(service_wrapper)
		{
		}

		/**
		 * get the underlying synchronous file io
		 */
		IFileIOPtr	sync_io() const
		{
			return sync_io_;
		}

		bool	exist(std::string const & file) override
		{
			return sync_io_->exist(file);
		}


		/** 
		 * Add a task into the queue and block until it's finished.
		 * If it's an urgent task and you don't want to pay the price of switching between threads or don't
		 *	want this task to be executed after all the previous tasks have been finished, call @a 
		 *	synio()->fulfill(file, buffer) instead.
		 * @see IFileIO::fulfill(std::string const & file, BufferPtr buffer)
		 * @exception std::system_error on failure
		 */
		void	fulfill(std::string const & file, BufferPtr buffer) override
		{
			std::promise<void> promise;
			std::future<void> future = promise.get_future();

			async_fulfill(file, buffer, [promise = std::move(promise)] (std::error_code err) mutable {
				if (err) {
					promise.set_exception(std::make_exception_ptr(std::system_error(err)));
				}
				else {
					promise.set_value();
				}
			});

			future.get();
		}

		template<class Handler>
		void	async_fulfill(std::string const & file, BufferPtr buffer, Handler && handler)
		{
			service_wrapper_.post([=, sync_io_ = sync_io_, handler = std::forward<Handler>(handler)]() mutable {
				try {
					sync_io_->fulfill(file, buffer);
					handler(std::error_code());
				}
				catch (std::system_error const & err) {
					handler(err.code());
				}
				catch (...) {
					handler(std::make_error_code(std::errc::io_error));
				}
			});
		}
	};
}