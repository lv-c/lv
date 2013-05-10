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

using namespace lv::net;


class RepeaterSession : public TcpSession
{
	typedef TcpSession	base_type;

	boost::shared_ptr<TcpSession>	dest_session_;

	string	dest_ip_;

	string	dest_port_;

	bool	dest_connected_;

	BufferPtr	cache_;

public:

	RepeaterSession(ContextPtr context, string const & dest_ip, string const & dest_port);

	virtual ~RepeaterSession();

private:

	virtual	void	on_connected();
	virtual	void	on_receive(BufferPtr buf);
	virtual	void	on_error(ErrorType type, boost::system::error_code const & error);

	void	dest_on_connected();
	void	dest_on_error(ErrorType type, boost::system::error_code const & error);
	void	dest_on_receive(BufferPtr buf);
};