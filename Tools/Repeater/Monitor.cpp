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
	, os_version_(WinOthers)
{
#ifdef LV_PLATFORM_WINDOWS

	OSVERSIONINFOEXA os;
	os.dwOSVersionInfoSize = sizeof(os);
	
	if (GetVersionExA((LPOSVERSIONINFOA) &os))
	{
		switch (os.dwMajorVersion)
		{
		case 6:
			os_version_ = Win2008;
			break;

		default:
			break;
		}
	}

	log::add_debug_string_gather(log_);

#endif

	LOG() << "os version:" << os_version_;

	boost::asio::ip::address_v4 mask = boost::asio::ip::address_v4::from_string(Config::instance().mask);
	ip_mask_ = mask.to_ulong();

	int bits = 0;
	for (int i = 0; i < sizeof(ip_mask_) * 8; ++i)
	{
		if(ip_mask_ & (1 << i))
		{
			bits ++;
		}
	}

	mask_bits_ = boost::lexical_cast<string>(bits);

	//
	start_timer();

	if (os_version_ == WinOthers)
	{
		ipsec("add mmpolicy name=lv_mmpolicy");	
	}

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

	if (masked_ip != 0)
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
	if (!error)
	{
		start_timer();
		hack_scan();
	}
}

void Monitor::hack_scan()
{
	Config const & cfg = Config::instance();

	for (IPStatMap::reference v : stat_)
	{
		IPStat & info = v.second;

		if (info.value[IPStat::RecvCount] == 0 && info.value[IPStat::SendCount] == 0
			&& info.value[IPStat::Connection] >= cfg.connections_to_ban
			&& info.sub_ip.size() >= cfg.sub_ip_to_ban)
		{
			boost::asio::ip::address_v4 addr(v.first);

			
			/*string str = (boost::format("add filter filterlist=lv_filterlist srcaddr=%s srcmask=%s dstaddr=me") %
				addr.to_string() % cfg.mask).str();
			*/

			if (os_version_ == WinOthers)
			{
				string str = (boost::format("add rule mmpolicy=lv_mmpolicy srcaddr=%s srcmask=%s dstaddr=Me actioninbound=block actionoutbound=block") %
					addr.to_string() % cfg.mask).str();
				
				ipsec(str);
			}
			else if (os_version_ == Win2008)
			{
				string str = "netsh advfirewall firewall add rule name=lv_repeater dir=in action=block remoteip=" + addr.to_string()
					+ "/" + mask_bits_;

				system(str.c_str());
			}

			log_() << "block ip:" << addr.to_string() << " connection:" << info.value[IPStat::Connection]
				<< " sub_ip:" << info.sub_ip.size();
		}
	}

	stat_.clear();
}

void Monitor::ipsec(string const & str)
{
#ifdef LV_PLATFORM_WINDOWS
	system(("netsh ipsec dynamic " + str).c_str());
#endif
}
