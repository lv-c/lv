#include "stdafx.h"
#include "RepeaterSession.hpp"
#include "Monitor.hpp"
#include "Config.hpp"

#include <lv/Log/DbgPrint.hpp>

#include <boost/asio/ip/address_v4.hpp>

RepeaterSession::RepeaterSession(ContextPtr context, string const & dest_ip, string const & dest_port, Monitor & monitor)
	: base_type(context)
	, dest_ip_(dest_ip)
	, dest_port_(dest_port)
	, monitor_(monitor)
	, dest_connected_(false)
	, remote_ip_(0)
{
}

RepeaterSession::~RepeaterSession()
{
	DbgPrint(LOG, "~RepeaterSession");
}

void RepeaterSession::on_connected()
{
	string ip = remote_ip();

	if(! ip.empty())
	{
		boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(ip);
		remote_ip_ = (addr.to_ulong());
	}


	//
	monitor_.increase(remote_ip_, IPStat::Connection, 1);

	dest_session_.reset(new TcpSession(context_));

	dest_session_->connect_event().connect(boost::bind(&RepeaterSession::dest_on_connected, this));
	dest_session_->error_event().connect(boost::bind(&RepeaterSession::dest_on_error, this, _1, _2));
	dest_session_->receive_event().connect(boost::bind(&RepeaterSession::dest_on_receive, this, _1));

	dest_session_->connect(dest_ip_, dest_port_);

	LOG() << "new sesssion:" << remote_ip();
}

void RepeaterSession::on_receive(BufferPtr buf)
{
	monitor_.increase(remote_ip_, IPStat::SendCount, 1);
	monitor_.increase(remote_ip_, IPStat::SendData, buf->size());


	if(dest_connected_)
	{
		dest_session_->start_write(buf);
	}
	else
	{
		if(cache_)
		{
			buffer::append(*cache_, *buf);
		}
		else
		{
			cache_ = buf;
		}
	}
}

void RepeaterSession::on_error(ErrorType type, boost::system::error_code const & error)
{
	LOG() << "on error:" << remote_ip();

	if(dest_session_)
	{
		dest_session_->shutdown();
		dest_session_->close();
	}

	close();
}

void RepeaterSession::dest_on_connected()
{
	monitor_.increase(remote_ip_, IPStat::DestConnectied, 1);

	//
	dest_connected_ = true;

	if(cache_)
	{
		dest_session_->start_write(cache_);
		cache_.reset();
	}
}

void RepeaterSession::dest_on_receive(BufferPtr buf)
{
	monitor_.increase(remote_ip_, IPStat::RecvCount, 1);
	monitor_.increase(remote_ip_, IPStat::RecvData, buf->size());

	start_write(buf);
}

void RepeaterSession::dest_on_error(ErrorType type, boost::system::error_code const & error)
{
	if(type == ErrorConnect)
	{
		LOG(lv::error) << "error connecting to dest:" << dest_ip_ << " port:" << dest_port_ << " msg:" << error.message();
	}
	else
	{
		LOG() << "dest session disconnected:" << remote_ip() << " msg:" << error.message();
	}

	shutdown();
	close();

	dest_session_->close();
}
