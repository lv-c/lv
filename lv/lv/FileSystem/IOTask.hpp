// *********************************************************************
//  IOTask   version:  1.0   ·  date: 08/09/2008
//  --------------------------------------------------------------------
//  IO 任务，  Monitor Object Pattern
//  使用异常(而不是返回结果)来传递是否执行成功
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_IOTASK_HPP
#define LV_IOTASK_HPP

#include <lv/lvlib2.hpp>
#include <lv/FileSystem/Fwd.hpp>
#include <lv/Buffer.hpp>
#include <lv/Concurrent/Future.hpp>

#include <boost/noncopyable.hpp>

#include <string>


namespace lv
{
	class IOTask : boost::noncopyable
	{
		typedef Promise<BufferPtr>	promise_type;

		std::shared_ptr<promise_type>	promise_;

		std::string		file_;

		BufferPtr		buffer_;

		IFileIOPtr		file_io_;

	public:

		IOTask(std::string file, BufferPtr buffer, IFileIOPtr file_io);

		IOTask(IOTask && other);


		/**
		 * Fulfill this task.
		 */
		void	operator () ();

		IOFuture	get_future();
		
	};
}


#endif