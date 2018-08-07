// *********************************************************************
//  MessageQueueOptions   version:  1.0   ��  date: 2015/11/18
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


namespace lv::net
{
	struct MessageQueueOptions
	{
		bool	preserve_order = true;

		double	resend_time = 20.0;
	};
}
