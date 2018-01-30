// *********************************************************************
//  SSLServer   version:  1.0   ¡¤  date: 04/25/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SSLSERVER_HPP
#define LV_NET_SSLSERVER_HPP

#include <lv/FrameWork/Net/ServerBase.hpp>


namespace lv::net
{
	class SSLServer : public ServerBase
	{
		using base_type = ServerBase;

		std::string	password_;

	public:

		SSLServer(ContextPtr context, creator_type session_creator = creator_type(), std::string const & password = "m2lvxu");

		/// @exception boost::system::system_error
		virtual	void	start(unsigned short port);

	protected:

		virtual	void	init_context();

		virtual	std::string	get_password();
	};

}

#endif