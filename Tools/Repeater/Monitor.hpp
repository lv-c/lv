// *********************************************************************
//  Stat   version:  1.0   ¡¤  date: 2014/04/04
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


struct IPStat
{
	enum Type
	{
		Connection,
		DestConnectied,
		SendCount,
		SendData,
		RecvCount,
		RecvData,
		Num
	};

	boost::array<uint64, Num>	value;

	set<uint32>	sub_ip;

	IPStat()
	{
		value.assign(0);
	}
};


class Monitor : boost::noncopyable
{
	typedef map<uint32, IPStat>	IPStatMap;

	IPStatMap	stat_;

	boost::asio::deadline_timer	timer_;

	Log		log_;

	uint32	ip_mask_;

public:

	Monitor(boost::asio::io_service & service);

	void	increase(uint32 ip, IPStat::Type type, uint32 num);

private:

	void	start_timer();

	void	on_timer(boost::system::error_code const & error);

	void	hack_scan();

	void	ipsec(string const & str);

};