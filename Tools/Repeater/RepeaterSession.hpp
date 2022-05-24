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

	shared_ptr<TcpSession>	dest_session_;

	string	dest_ip_;

	string	dest_port_;

	bool	dest_connected_ = false;

	BufferPtr	cache_;

	Monitor &	monitor_;

	uint32_t	remote_ip_ = 0;

	Timer		active_timer_;

	boost::asio::deadline_timer	timer_;

public:

	RepeaterSession(ContextPtr context, string const & dest_ip, string const & dest_port, Monitor & monitor);

	~RepeaterSession() override;

private:

	void	start_timer();

	void	on_timer(boost::system::error_code const & error);

	void	exit();

	void	on_connected() override;
	void	on_receive(Buffer const & buf) override;
	void	on_error(ErrorType type, boost::system::error_code const & error) override;

	void	dest_on_connected();
	void	dest_on_error(ErrorType type, boost::system::error_code const & error);
	void	dest_on_receive(Buffer const & buf);
};
