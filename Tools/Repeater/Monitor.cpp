#include "stdafx.h"
#include "Monitor.hpp"
#include "Config.hpp"

#include <lv/StreamHelper.hpp>
#include <lv/Log/Utility.hpp>

#include <boost/bind.hpp>
#include <boost/asio/ip/address_v4.hpp>


Monitor::Monitor(boost::asio::io_service & service)
	: timer_(service)
	, ip_mask_(0)
{
	boost::asio::ip::address_v4 mask = boost::asio::ip::address_v4::from_string(Config::instance().mask);
	ip_mask_ = mask.to_ulong();

	//
	log::add_debug_string_gather(log_);
	start_timer();

	ipsec("add mmpolicy name=lv_mmpolicy");

	/*
	ipsec("add policy name=lv_policy");
	ipsec("add filterlist name=lv_filterlist");

	ipsec("add filteraction name=lv_filteraction action=block");

	ipsec("add rule name=kill_hack policy=lv_policy filterlist=lv_filterlist filteraction=lv_filteraction");
	ipsec("set policy name=lv_policy assign=y");
	*/
}

void Monitor::increase(uint32 ip, IPStat::Type type, uint32 num)
{
	uint32 masked_ip = ip & ip_mask_;

	if(masked_ip != 0)
	{
		IPStat & s = stat_[masked_ip];
		s.value[type] += num;
		s.sub_ip.insert(ip & (~ip_mask_));
	}
}

void Monitor::start_timer()
{
	timer_.expires_from_now(boost::posix_time::minutes(Config::instance().stat_minutes));
	timer_.async_wait(boost::bind(&Monitor::on_timer, this, _1));
}

void Monitor::on_timer(boost::system::error_code const & error)
{
	if(! error)
	{
		start_timer();
		hack_scan();
	}
}

void Monitor::hack_scan()
{
	Config const & cfg = Config::instance();

	foreach(IPStatMap::reference v, stat_)
	{
		IPStat & info = v.second;

		if(info.value[IPStat::RecvCount] == 0 && info.value[IPStat::SendCount] == 0
			&& info.value[IPStat::Connection] >= cfg.connections_to_ban
			&& info.sub_ip.size() >= cfg.sub_ip_to_ban)
		{
			boost::asio::ip::address_v4 addr(v.first);

			
			/*string str = (boost::format("add filter filterlist=lv_filterlist srcaddr=%s srcmask=%s dstaddr=me") %
				addr.to_string() % cfg.mask).str();
			*/

			string str = (boost::format("add rule mmpolicy=lv_mmpolicy srcaddr=%s srcmask=%s dstaddr=Me actioninbound=block actionoutbound=block") %
				addr.to_string() % cfg.mask).str();
				
			ipsec(str);

			log_() << "block ip:" << addr.to_string() << " connection:" << info.value[IPStat::Connection]
				<< " sub_ip:" << info.sub_ip.size();
		}
	}

	stat_.clear();
}

void Monitor::ipsec(string const & str)
{
	system(("netsh ipsec dynamic " + str).c_str());
}