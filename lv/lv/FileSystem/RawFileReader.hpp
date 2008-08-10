// *********************************************************************
//  RawFileReader   version:  1.0   ·  date: 08/09/2008
//  --------------------------------------------------------------------
//  直接读取磁盘文件
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RAWFILEREADER_HPP
#define LV_RAWFILEREADER_HPP

#include <lv/FileSystem/IFileIO.hpp>

namespace lv
{
	class RawFileReader : public IFileIO
	{
	public:

		// throws file_io_error on failure
		virtual void	fulfill(std::string const & file, BufferPtr buffer);
	};
}

#endif // LV_RAWFILEREADER_HPP