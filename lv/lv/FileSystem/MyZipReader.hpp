// *********************************************************************
//  MyZipReader   version:  1.0   ¡¤  date: 08/22/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Config.hpp>
#include <lv/FileSystem/Fwd.hpp>
#include <lv/FileSystem/IFileIO.hpp>

#include <map>
#include <mutex>


namespace lv
{
	class MyUnzip;

	class MyZipReader : public IFileIO
	{
		using UnzipPtr = std::shared_ptr<MyUnzip>;
		using UnzipMap = std::map<std::string, UnzipPtr>;

		UnzipMap	unzip_;

		std::string	password_;

		std::string	postfix_;

		bool	in_memory_;

		IFileIOPtr	raw_file_reader_;

		using lock_guard = std::lock_guard<std::mutex>;

		std::mutex	mutex_;

	public:

		/**
		 * @password password for ( all the packet files
		 * @in_memory load all the packet files into memory
		 */
		MyZipReader(std::string const & password, std::string const & working_dir, std::string const & postfix, bool in_memory = false);

		~MyZipReader();

		/** 
		 * @see IFileIO::fulfill
		 */
		void	fulfill(std::string const & file, BufferPtr buf) override;

		bool	exist(std::string const & file) override;


		void	reset() override;

	private:

		/** 
		 * get the lower-case name of the zip file which contains the file specified by @file
		 * @return returns "" to indicate an error
		 */
		std::string	get_zip_file(std::string const & file);


		UnzipPtr	get_unzip(std::string const & file, std::string & inner_path);
	};
}
