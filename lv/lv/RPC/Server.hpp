// *********************************************************************
//  Server   version:  1.0   ¡¤  date: 09/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_SERVER_HPP
#define LV_RPC_SERVER_HPP


namespace lv { namespace rpc {

	class IPacket;

	template<typename Id, class IArchive = IPacket>
	class Server
	{

	public:

		template<class F>
		Server & reg(Id const & id, F f);
	};

} }
#endif // LV_RPC_SERVER_HPP