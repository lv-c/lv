// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 11/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/fusion/include/map_fwd.hpp>

#include <memory>


namespace lv::rpc
{
	struct PacketArchive;
	
	template<
		class Id = std::string, 
		class ArchivePair = PacketArchive
	>
	class Client;

	template<
		class Id = std::string, 
		class ArchivePair = PacketArchive
	>
	class Server;

}