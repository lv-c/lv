// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 08/22/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Buffer.hpp>


namespace lv
{
	class IFileIO;
	using IFileIOPtr = std::shared_ptr<IFileIO>;

	class RawFileReader;
	class RawFileWriter;

	class MyZipReader;

	class AsyncFileIO;
}
