// *********************************************************************
//  AsynFileIO   version:  1.0   ·  date: 08/09/2008
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
#include <lv/ServiceWrapper.hpp>

namespace lv
{
	class AsynFileIO : public IFileIO
	{
	public:
		typedef std::shared_ptr<IFileIO>	synio_ptr;

	private:

		synio_ptr	syn_filio_;

		ServiceWrapper	service_wrapper_;

	public:
		
		AsynFileIO(synio_ptr synio, ServiceWrapper const & service_wrapper)
			: syn_filio_(synio)
			, service_wrapper_(service_wrapper)
		{
		}

		/**
		 * get the underlying synchronous file io
		 */
		synio_ptr	syn_io() const
		{
			return syn_filio_;
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
			IOFuture future = add_task(file, buffer);

			try
			{
				future();
			}
			catch (std::runtime_error const & err)
			{
				throw file_io_error(err.what());
			}
		}

		virtual	IOFuture add_task(std::string const & file, BufferPtr buffer)
		{
			IOTask task(file, buffer, syn_filio_);

			service_wrapper_.post(task);
			
			return IOFuture(task);
		}
	};
}

#endif