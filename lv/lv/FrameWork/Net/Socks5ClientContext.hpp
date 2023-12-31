// *********************************************************************
//  Socks5ClientContext   version:  1.0   ��  date: 10/14/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/Context.hpp>

#include <string>


namespace lv::net
{
	class Socks5ClientContext : public Context
	{
		std::string	proxy_;
		std::string	port_;

		std::string	user_;
		std::string	password_;

	public:

		Socks5ClientContext(IOContextWrapper const & io_wrapper)
			: Context(io_wrapper)
		{
		}

		void	set_proxy(std::string const & proxy, std::string const & port)
		{
			this->proxy_ = proxy;
			this->port_ = port;
		}

		void	set_auth(std::string const & user, std::string const & password)
		{
			this->user_ = user;
			this->password_ = password;
		}

		void	get_proxy(std::string & proxy, std::string & port) const
		{
			proxy = this->proxy_;
			port = this->port_;
		}

		void	get_auth(std::string & user, std::string & password) const
		{
			user = this->user_;
			password = this->password_;
		}

		bool	has_auth() const
		{
			return (!user_.empty());
		}

	};

}