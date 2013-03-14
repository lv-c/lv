// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 11/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_FWD_HPP
#define LV_RPC_FWD_HPP

#include <boost/fusion/include/map_fwd.hpp>
#include <boost/shared_ptr.hpp>


namespace lv { namespace rpc {

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

} }

#endif