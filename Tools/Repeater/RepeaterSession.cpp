#include "stdafx.h"
#include "RepeaterSession.hpp"

#include <lv/Log/DbgPrint.hpp>

RepeaterSession::RepeaterSession(ContextPtr context, string const & dest_ip, string const & dest_port)
	: base_type(context)
	, dest_ip_(dest_ip)
	, dest_port_(dest_port)
	, dest_connected_(false)
{
}

RepeaterSession::~RepeaterSession()
{
	DbgPrint(LOG, "~RepeaterSession");
}

void RepeaterSession::on_connected()
{
	dest_session_.reset(new TcpSession(context_));

	dest_session_->connect_event().connect(boost::bind(&RepeaterSession::dest_on_connected, this));
	dest_session_->error_event().connect(boost::bind(&RepeaterSession::dest_on_error, this, _1, _2));
	dest_session_->receive_event().connect(boost::bind(&RepeaterSession::dest_on_receive, this, _1));

	dest_session_->connect(dest_ip_, dest_port_);

	LOG() << "new sesssion:" << remote_ip();
}

void RepeaterSession::on_receive(BufferPtr buf)
{
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
	dest_connected_ = true;

	if(cache_)
	{
		dest_session_->start_write(cache_);
		cache_.reset();
	}
}

void RepeaterSession::dest_on_receive(BufferPtr buf)
{
	start_write(buf);
}

void RepeaterSession::dest_on_error(ErrorType type, boost::system::error_code const & error)
{
	LOG() << "dest session disconnected:" << remote_ip();

	shutdown();
	close();

	dest_session_->close();
}
