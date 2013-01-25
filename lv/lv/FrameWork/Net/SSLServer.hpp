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

namespace lv { namespace net {


	class SSLContextHolder;

	class SSLServer : public ServerBase
	{
		typedef ServerBase	base_type;

		// using scoped_ptr here will lead to C2027 : use of undefined type 'lv::net::SSLContextHolder'. Don't know why
		boost::shared_ptr<SSLContextHolder>	ssl_context_;
		
		std::string	password_;

	public:

		SSLServer(ContextPtr context, creator_type session_creator = creator_type(), std::string const & password = "rswvfbuk");

		/// @exception boost::system::system_error
		virtual	void	start(unsigned short port);

	protected:

		virtual	void	init_context();

		virtual	std::string	get_password();
	};

} }

#endif