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

	struct Protocol;
	struct PacketArchive;

	class ISocket;
	typedef boost::shared_ptr<ISocket> SocketPtr;

	class IBufferManager;
	typedef boost::shared_ptr<IBufferManager> BufferManagerPtr;


	template<
		typename Id = std::string, 
		class ArchivePair = PacketArchive, 
		class Pro = Protocol
	>
	class Client;

	template<
		class ParamExtractors = boost::fusion::map<>, 
		typename Id = std::string, 
		class ArchivePair = PacketArchive, 
		class Pro = Protocol
	>
	class Registery;

	template<class Reg = Registery<> >
	class Server;


	template<
		class ArchivePair = PacketArchive, 
		class Pro = Protocol
	>
	class Exceptions;
	
} }

#endif // LV_RPC_FWD_HPP