// *********************************************************************
//  RawFileWriter   version:  1.0   ��  date: 08/09/2008
//  --------------------------------------------------------------------
//	ֱ�Ӱ��ļ�д������
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RAWFILEWRITER_HPP
#define LV_RAWFILEWRITER_HPP

#include <lv/FileSystem/IFileIO.hpp>

namespace lv
{
	class RawFileWriter : public IFileIO
	{
	public:
		// throws file_io_error on failure
		virtual void	fulfill(std::string const & file, BufferPtr buffer);
	};
}

#endif // LV_RAWFILEWRITER_HPP