// *********************************************************************
//  MyZipReader   version:  1.0   ¡¤  date: 08/22/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_MYZIPREADER_HPP
#define LV_MYZIPREADER_HPP

#include <lv/FileSystem/IFileIO.hpp>

#include <boost/thread/mutex.hpp>

#include <string>
#include <map>

namespace lv
{
	class MyUnzip;

	class MyZipReader : public IFileIO
	{
		typedef boost::shared_ptr<MyUnzip>	UnzipPtr;
		typedef std::map<std::string, UnzipPtr>	UnzipMap;

		UnzipMap	unzip_;

		std::string	password_;

		std::string	postfix_;

		bool	in_memory_;

		IFileIOPtr	raw_file_reader_;

		typedef boost::mutex::scoped_lock	scoped_lock;
		boost::mutex	mutex_;

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
		virtual void	fulfill(std::string const & file, BufferPtr buf);

		virtual	bool	exist(std::string const & file);


		virtual	void	reset();

	private:

		/** 
		 * get the lower-case name of the zip file which contains the file specified by @file
		 * @return returns "" to indicate an error
		 */
		std::string	get_zip_file(std::string const & file);


		UnzipPtr	get_unzip(std::string const & file, std::string & inner_path);
	};
}


#endif