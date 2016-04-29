// *********************************************************************
//  CallForwarding   version:  1.0   ¡¤  date: 04/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAM_CALLFORWARDING_HPP
#define LV_STREAM_CALLFORWARDING_HPP

namespace lv
{

#define LV_STREAMPROXY_FW_CALL_0(call, ret)	\
	if (std_ios_ != NULL)		\
	{							\
		ret std_ios_->call();	\
	}							\
	else						\
	{							\
		ret lv_ios_->call();	\
	}


#define LV_STREAMPROXY_FW_CALL_1(call, arg0, ret)	\
	if (std_ios_ != NULL)			\
	{								\
		ret std_ios_->call(arg0);	\
	}								\
	else							\
	{								\
		ret lv_ios_->call(arg0);	\
	}


#define LV_STREAMPROXY_FW_CALL_2(call, arg0, arg1, ret)	\
	if (std_ios_ != NULL)				\
	{									\
		ret std_ios_->call(arg0, arg1);	\
	}									\
	else								\
	{									\
		ret lv_ios_->call(arg0, arg1);	\
	}

}

#endif