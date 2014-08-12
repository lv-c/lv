// *********************************************************************
//  RepeaterSession   version:  1.0   ¡¤  date: 2013/05/09
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/TcpSession.hpp>
#include <lv/Timer.hpp>

using namespace lv::net;

class Monitor;

class RepeaterSession : public TcpSession
{
	typedef TcpSession	base_type;

	boost::shared_ptr<TcpSession>	dest_session_;

	string	dest_ip_;

	string	dest_port_;

	bool	dest_connected_;

	BufferPtr	cache_;

	Monitor &	monitor_;

	uint32	remote_ip_;

	Timer	active_timer_;

	boost::asio::deadline_timer	timer_;

public:

	RepeaterSession(ContextPtr context, string const & dest_ip, string const & dest_port, Monitor & monitor);

	virtual ~RepeaterSession();

private:

	void	start_timer();

	void	on_timer(boost::system::error_code const & error);

	void	exit();

	virtual	void	on_connected();
	virtual	void	on_receive(BufferPtr buf);
	virtual	void	on_error(ErrorType type, boost::system::error_code const & error);

	void	dest_on_connected();
	void	dest_on_error(ErrorType type, boost::system::error_code const & error);
	void	dest_on_receive(BufferPtr buf);
};