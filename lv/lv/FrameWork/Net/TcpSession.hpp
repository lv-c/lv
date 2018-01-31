// *********************************************************************
//  TcpSession   version:  1.0   ¡¤  date: 01/03/2010
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
	class TcpSession : public SessionBase
	{
		using base_type = SessionBase;

	public:

		explicit TcpSession(ContextPtr context);
	};

}