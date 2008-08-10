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

#include <string>
#include <boost/future.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/operators.hpp>

#include <lv/Buffer.hpp>

namespace lv
{
	class IFileIO;

	class IOTask : public boost::promise<void>, boost::equality_comparable<IOTask>
	{
		typedef boost::shared_ptr<std::string> string_ptr;
		string_ptr	file_;
		BufferPtr	buffer_;

		boost::shared_ptr<IFileIO>	file_io_;

	public:

		IOTask(std::string const & file, BufferPtr buffer, boost::shared_ptr<IFileIO> file_io)
			: file_(new std::string(file))
			, buffer_(buffer)
			, file_io_(file_io)
		{
		}

		IOTask(string_ptr file, BufferPtr buffer, boost::shared_ptr<IFileIO> file_io)
			: file_(file)
			, buffer_(buffer)
			, file_io_(file_io)
		{
		}


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


#endif // LV_IOTASK_HPP