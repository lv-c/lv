// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 08/22/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FILESYSTEM_HPP
#define LV_FILESYSTEM_HPP

#include <boost/shared_ptr.hpp>

namespace lv
{
	class IFileIO;
	typedef boost::shared_ptr<IFileIO>	IFileIOPtr;

	class RawFileReader;
	class RawFileWriter;

	class MyZipReader;

	class IOFuture;

	class IOTask;
}


#endif