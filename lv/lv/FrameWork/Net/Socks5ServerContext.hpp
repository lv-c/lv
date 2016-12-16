// *********************************************************************
//  Socks5ServerContext   version:  1.0   ¡¤  date: 11/04/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SOCKS5SERVERCONTEXT_HPP
#define LV_NET_SOCKS5SERVERCONTEXT_HPP

#include <lv/FrameWork/Net/Context.hpp>
#include <lv/FrameWork/Net/Socks5.hpp>

#include <vector>
#include <set>
#include <algorithm>

namespace lv { namespace net {

	struct Socks5Auth
	{
		std::string	user;
		std::string	password;

		bool operator < (Socks5Auth const & rhs) const
		{
			if (user == rhs.user)
			{
				return password < rhs.password;
			}
			else
			{
				return user < rhs.user;
			}
		}
	};


	class ISocks5ServerContext : public Context
	{
		std::string	address_to_bind_;

	public:

		ISocks5ServerContext(BufferManagerPtr buf_manager, ServiceWrapper const & service_wrapper)
			: Context(buf_manager, service_wrapper)
		{
		}

		void	address_to_bind(std::string const & addr)
		{
			address_to_bind_ = addr;
		}

		std::string const & address_to_bind() const
		{
			return address_to_bind_;
		}

		
		virtual	uint8	select_method(std::vector<uint8> const & methods) = 0;

		virtual	bool	verify(Socks5Auth const & auth) = 0;
	};

	
	class Socks5ServerContext : public ISocks5ServerContext
	{
		std::set<Socks5Auth>	auth_;

	public:

		Socks5ServerContext(BufferManagerPtr buf_manager, ServiceWrapper const & service_wrapper)
			: ISocks5ServerContext(buf_manager, service_wrapper)
		{
		}

		void	set_auth(std::set<Socks5Auth> const & auth)
		{
			this->auth_ = auth;
		}

		uint8	select_method(std::vector<uint8> const & methods) override
		{
			uint8 m = (auth_.empty() ? Socks5::NoAuth : Socks5::UserPassword);

			if (std::find(methods.begin(), methods.end(), m) == methods.end())
			{
				return Socks5::NoAcceptable;
			}
			else
			{
				return m;
			}
		}

		bool	verify(Socks5Auth const & auth) override
		{
			BOOST_ASSERT(! auth_.empty());

			return (this->auth_.find(auth) != this->auth_.end());
		}

	};

} }

#endif