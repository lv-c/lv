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
#include <lv/Concurrent/ThreadPool.hpp>

namespace lv
{
	class AsynFileIO : public IFileIO
	{
		typedef boost::shared_ptr<IFileIO>	synio_ptr;
		synio_ptr	syn_filio_;

		ThreadPool	pool_;

	public:
		// throw(boost::thread_resource_error) (only if initial_threads > 0)
		AsynFileIO(synio_ptr synio, size_t initial_threads)
			: syn_filio_(synio)
			, pool_(initial_threads)
		{
		}

		/** 
		 * Add a task into the queue and block until it's finished.
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
			catch(std::runtime_error const & err)
			{
				throw file_io_error(err.what());
			}
		}

		virtual	IOFuture add_task(std::string const & file, BufferPtr buffer)
		{
			IOTask task(file, buffer, syn_filio_);
			pool_.add_task(task);
			return IOFuture(task);
		}


		/**
		 * Clear all the pending tasks
		 */
		void	clear_tasks()
		{
			pool_.clear();
		}

		size_t	get_thread_num()
		{
			return pool_.size();
		}

		/**
		 * @see ThreadPool::resize()
		 */ 
		void	set_thread_num(size_t num, bool wait = false)
		{
			pool_.resize(num, wait);
		}

	};
}

#endif // LV_ASYNFILEIO_HPP