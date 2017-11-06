// *********************************************************************
//  TcpSession   version:  1.0   ¡¤  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_TCPSESSION_HPP
#define LV_NET_TCPSESSION_HPP

#include <lv/FrameWork/Net/SessionBase.hpp>


namespace lv::net
{
	class TcpSession : public SessionBase
	{
		typedef SessionBase	base_type;

	public:

		explicit TcpSession(ContextPtr context);
	};

}

#endif