// *********************************************************************
//  IFileIO   version:  1.0   ·  date: 08/09/2008
//  --------------------------------------------------------------------
//		文件系统接口
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/lvlib2.hpp>
#include <lv/Buffer.hpp>

#include <string>


namespace lv
{
	class IFileIO
	{

		std::string		working_dir_;

	public:

		explicit IFileIO(std::string const & working_dir = std::string());

		virtual ~IFileIO() {}

		/**
		 * Synchronous operation. 
		 * @exception std::system_error on failure
		 */
		virtual void	fulfill(std::string const & file, BufferPtr buf) = 0;

		virtual	bool	exist(std::string const & file) = 0;


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