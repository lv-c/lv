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

#include <lv/FileSystem/Fwd.hpp>
#include <lv/Buffer.hpp>
#include <lv/lvlib2.hpp>

#include <boost/operators.hpp>

#include <future>
#include <string>
#include <memory>

namespace lv
{
	class IOTask : boost::equality_comparable<IOTask>
	{
		friend class IOFuture;

		typedef std::promise<void>	promise_type;

		std::shared_ptr<promise_type>	promise_;

		typedef std::shared_ptr<std::string> string_ptr;
		string_ptr	file_;
		BufferPtr	buffer_;

		std::shared_ptr<IFileIO>	file_io_;

	public:

		IOTask(std::string const & file, BufferPtr buffer, std::shared_ptr<IFileIO> file_io);


		/**
		 * Retrieves the buffer
		 */
		BufferPtr	buffer() const
		{
			return buffer_;
		}


		std::string	const & file() const
		{
			return *file_;
		}

		std::future<void>	get_future()
		{
			return std::move(promise_->get_future());
		}


		/**
		 * Fulfill this task.
		 */
		void	operator () ();


		friend bool operator == (IOTask const & lhs, IOTask const & rhs)
		{
			return (lhs.file_ == rhs.file_) && (lhs.buffer_ == rhs.buffer_) && 
				(lhs.file_io_ == rhs.file_io_);
		}

	};
}


#endif