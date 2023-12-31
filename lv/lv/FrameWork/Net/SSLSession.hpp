// *********************************************************************
//  SSLSession   version:  1.0   ��  date: 04/25/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/SessionBase.hpp>


namespace lv::net
{
	class SSLSession : public SessionBase
	{
		using base_type = SessionBase;

		enum class HandshakeType
		{
			Client,
			Server
		};

	public:

		SSLSession(ContextPtr context);

		//
		void	server_side_start() override;

	protected:

		virtual	void	handle_handshake(boost::system::error_code const & error);

		void	handle_connect(boost::system::error_code const & error) override;

	private:

		void	handshake(HandshakeType type);

	};


}
