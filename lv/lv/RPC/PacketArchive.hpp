// *********************************************************************
//  PacketArchive   version:  1.0   ¡¤  date: 09/18/2008
//  --------------------------------------------------------------------
//	A portable binary archive type.
//
//	see OPacket.hpp
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_PACKETARCHIVE_HPP
#define LV_RPC_PACKETARCHIVE_HPP

#include <lv/Serialization/IArchive.hpp>
#include <lv/Serialization/OArchive.hpp>

#include <lv/RPC/ArchiveBase.hpp>

namespace lv { namespace rpc {

	struct PacketArchive : ArchiveBase<IArchive, OArchive>
	{
	};

} }


#endif