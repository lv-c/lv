// *********************************************************************
//  Socks5   version:  1.0   ¡¤  date: 11/04/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


namespace lv::net
{
	struct Socks5
	{
		static uint8_t const	Version		= 0x05;

		static uint8_t const	AuthVersion	= 0x01;

		enum Method : uint8_t
		{
			NoAuth			=	0,
			UserPassword	=	2,
			NoAcceptable	=	255
		};

		enum AddrType : uint8_t
		{
			IPV4		=	1,
			DomainName	=	3,
			IPV6		=	4
		};
	};

}