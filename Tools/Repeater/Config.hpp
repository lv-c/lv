// *********************************************************************
//  Config   version:  1.0   ¡¤  date: 2014/04/04
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/serialization/nvp.hpp>

struct Address
{
	string		ip;
	uint16_t	port;

	template<class Archive>
	void	serialize(Archive & ar, unsigned int)
	{
		ar & ip & port;
	}
};

struct Config : public Singleton<Config>
{
	typedef map<uint16_t, Address> AddressMap;
	AddressMap	mapping;

	string		mask;

	uint32_t	stat_minutes;

	uint32_t	no_data_disconnect_seconds;

	uint32_t	sub_ip_to_ban;

	uint32_t	connections_to_ban;

	template<typename Archive>
	void	serialize(Archive & ar, unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(mapping)
			& BOOST_SERIALIZATION_NVP(mask)
			& BOOST_SERIALIZATION_NVP(stat_minutes)
			& BOOST_SERIALIZATION_NVP(sub_ip_to_ban)
			& BOOST_SERIALIZATION_NVP(connections_to_ban)
			& BOOST_SERIALIZATION_NVP(no_data_disconnect_seconds);
	}

};