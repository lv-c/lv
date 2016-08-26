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

#ifndef LV_ASYNFILEIO_HPP
#define LV_ASYNFILEIO_HPP

#include <lv/FileSystem/IFileIO.hpp>
#include <lv/FileSystem/IOTask.hpp>
#include <lv/ServiceWrapper.hpp>
#include <lv/Exception.hpp>


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

		virtual	bool	exist(std::string const & file)
		{
			return sync_io_->exist(file);
		}


		/** 
		 * Add a task into the queue and block until it's finished.
		 * If it's an urgent task and you don't want to pay the price of switching between threads or don't
		 *	want this task to be executed after all the previous tasks have been finished, call @a 
		 *	synio()->fulfill(file, buffer) instead.
		 * @see IFileIO::fulfill(std::string const & file, BufferPtr buffer)
		 * @exception lv::file_io_error on failure
		 */
		virtual void fulfill(std::string const & file, BufferPtr buffer)
		{
			add_task(file, buffer).get();
		}

		IOFuture	add_task(std::string const & file, BufferPtr buffer)
		{
			IOTask task(file, buffer, sync_io_);
			IOFuture future = task.get_future();

			service_wrapper_.post(std::move(task));
			
			return future;
		}
	};
}

#endif