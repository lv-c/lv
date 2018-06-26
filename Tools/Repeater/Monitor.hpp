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

	array<uint64_t, Num>	value;

	set<uint32_t>	sub_ip;

	IPStat()
	{
		value.fill(0);
	}
};


class Monitor : boost::noncopyable
{
	typedef map<uint32_t, IPStat>	IPStatMap;

	IPStatMap	stat_;

	boost::asio::deadline_timer	timer_;

	Log			log_;

	uint32_t	ip_mask_;

	string		mask_bits_;

	enum OSVerion
	{
		Win2008,
		WinOthers
	};

	OSVerion	os_version_;

public:

	Monitor(boost::asio::io_service & service);

	void	increase(uint32_t ip, IPStat::Type type, uint32_t num);

private:

	void	start_timer();

	void	on_timer(boost::system::error_code const & error);

	void	hack_scan();

	void	ipsec(string const & str);

};
