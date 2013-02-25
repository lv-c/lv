// *********************************************************************
//  SSLSession   version:  1.0   ¡¤  date: 04/25/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SSLSESSION_HPP
#define LV_NET_SSLSESSION_HPP

#include <lv/FrameWork/Net/SessionBase.hpp>

namespace lv { namespace net {

	class SSLSession : public SessionBase
	{
		typedef SessionBase	base_type;

		enum HandshakeType
		{
			ClientHandshake,
			ServerHandshake
		};

	public:

		SSLSession(ContextPtr context);

		//
		virtual	void	server_side_start();

	protected:

		virtual void	handle_handshake(boost::system::error_code const & error);

		virtual	void	handle_connect(boost::system::error_code const & error);

	private:

		void	handshake(HandshakeType type);

	};


} }

#endif