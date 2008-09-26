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

#include <lv/Packet/IPacket.hpp>
#include <lv/Packet/OPacket.hpp>

#include <lv/RPC/ArchiveBase.hpp>

namespace lv { namespace rpc {
{
	struct PacketArchive : ArchiveBase<IPacket, OPacket>
	{
	};

} }


#endif // LV_RPC_PACKETARCHIVE_HPP