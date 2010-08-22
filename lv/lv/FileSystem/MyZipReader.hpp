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

#include <lv/MyZip.hpp>

#include <boost/thread/mutex.hpp>

#include <string>
#include <map>

namespace lv
{
	class MyZipReader : public IFileIO
	{
		typedef boost::shared_ptr<MyUnzip>	unzip_ptr;
		typedef std::map<std::string, unzip_ptr>	unzip_map;

		unzip_map	unzip_;

		std::string	path_;

		std::string	password_;

		std::string	postfix_;

		typedef boost::mutex::scoped_lock	scoped_lock;
		boost::mutex	mutex_;

	public:

		/**
		 * @password password for all the packet files
		 */
		MyZipReader(std::string const & password, std::string const & path, std::string const & postfix);

		~MyZipReader();

		/** 
		 * @see IFileIO::fulfill
		 */
		virtual void	fulfill(std::string const & file, BufferPtr buf);


		// close all the zip files (automatically reopen them when they are needed)
		void	close();

	private:

		/** 
		 * get the lower-case name of the zip file which contains the file specified by @file
		 * @return returns "" to indicate an error
		 */
		std::string	get_zip_file(std::string const & file);
	};
}


#endif