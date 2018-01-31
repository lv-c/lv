// *********************************************************************
//  RawFileWriter   version:  1.0   ��  date: 08/09/2008
//  --------------------------------------------------------------------
//	ֱ�Ӱ��ļ�д������
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FileSystem/IFileIO.hpp>

namespace lv
{
	class RawFileWriter : public IFileIO
	{
	public:

		RawFileWriter(std::string const & working_dir = std::string());

		// throws std::system_error on failure
		void	fulfill(std::string const & file, BufferPtr buffer) override;

		bool	exist(std::string const & file) override;
	};
}