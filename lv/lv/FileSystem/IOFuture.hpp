// *********************************************************************
//  IOFuture   version:  1.0   ¡¤  date: 08/09/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_IOFUTURE_HPP
#define LV_IOFUTURE_HPP

#include <lv/FileSystem/IOTask.hpp>

namespace lv
{
	class IOFuture : public boost::future<void>, boost::equality_comparable<IOFuture>
	{
		boost::shared_ptr<std::string const>	file_;
		BufferPtr	buffer_;

	public:

		IOFuture(IOTask const & task)
			: file_(task.file_)
			, buffer_(task.buffer_)
			, boost::future<void>(task)
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


		friend bool operator == (IOFuture const & lhs, IOFuture const & rhs)
		{
			return lhs.file_ == rhs.file_ && lhs.buffer_ == rhs.buffer_;
		}

	};
}

#endif // LV_IOFUTURE_HPP