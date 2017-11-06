// *********************************************************************
//  Socks5   version:  1.0   ¡¤  date: 11/04/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SOCKS5_HPP
#define LV_NET_SOCKS5_HPP

#include <lv/IntType.hpp>


namespace lv::net
{
	struct Socks5
	{
		static uint8 const	Version		= 0x05;

		static uint8 const	AuthVersion	= 0x01;

		enum Method : uint8
		{
			NoAuth			=	0,
			UserPassword	=	2,
			NoAcceptable	=	255
		};

		enum AddrType : uint8
		{
			IPV4		=	1,
			DomainName	=	3,
			IPV6		=	4
		};
	};

}

#endif