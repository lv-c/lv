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

#include <lv/Buffer.hpp>


namespace lv
{
	class IFileIO;
	typedef std::shared_ptr<IFileIO>	IFileIOPtr;

	class RawFileReader;
	class RawFileWriter;

	class MyZipReader;

	class IOTask;

	template<typename Ret> class Future;
	typedef Future<BufferPtr>	IOFuture;
}


#endif