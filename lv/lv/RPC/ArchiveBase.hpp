// *********************************************************************
//  ArchiveBase   version:  1.0   ��  date: 09/18/2008
//  --------------------------------------------------------------------
//  A helper class for ( declaring archive types.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


namespace lv::rpc
{
	template<class IArchive, class OArchive>
	struct ArchiveBase
	{
		using iarchive_type = IArchive;
		using oarchive_type = OArchive;
	};

}
