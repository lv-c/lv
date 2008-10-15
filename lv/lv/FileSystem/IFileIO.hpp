// *********************************************************************
//  IFileIO   version:  1.0   ·  date: 08/09/2008
//  --------------------------------------------------------------------
//		文件系统接口
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_IFILEIO_HPP
#define LV_IFILEIO_HPP

#include <boost/enable_shared_from_this.hpp>
#include <lv/FileSystem/IOFuture.hpp>

namespace lv
{
	class IFileIO : public boost::enable_shared_from_this<IFileIO>
	{
	public:

		/**
		 * Synchronous operation. 
		 * @exception lv::file_io_error on failure
		 */
		virtual void	fulfill(std::string const & file, BufferPtr buffer) = 0;

		/**
		 * This should perform an asynchronous operation if it's an asynchronous io class.
		 * Otherwise it should call @a fulfill to perform a synchronous operation, which is
		 * the default behavior.
		 */
		virtual	IOFuture	add_task(std::string const & file, BufferPtr buffer);
	};
}

#endif