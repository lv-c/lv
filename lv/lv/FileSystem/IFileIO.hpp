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

#include <lv/FileSystem/Fwd.hpp>
#include <lv/FileSystem/IOFuture.hpp>


namespace lv
{
	class IFileIO : public std::enable_shared_from_this<IFileIO>
	{

		std::string		working_dir_;

	public:

		explicit IFileIO(std::string const & working_dir = std::string());

		virtual ~IFileIO() {}

		/**
		 * Synchronous operation. 
		 * @exception lv::file_io_error on failure
		 */
		virtual void	fulfill(std::string const & file, BufferPtr buf) = 0;

		virtual	bool	exist(std::string const & file) = 0;

		/**
		 * This should perform an asynchronous operation if it's an asynchronous io class.
		 * Otherwise it should call @a fulfill to perform a synchronous operation, which is
		 * the default behavior.
		 */
		virtual	IOFuture	add_task(std::string const & file, BufferPtr buf);


		/**
		 * Reset the loader
		 */
		virtual	void	reset() {}

	protected:

		// post condition: buf.size() == len, buf.capacity() > buf.size(), buf[len] = '\0'
		static void	init_buffer(Buffer & buf, size_t len);

		std::string	resolve(std::string const & file);

	};
}

#endif