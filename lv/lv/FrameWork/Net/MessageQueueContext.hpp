// *********************************************************************
//  MessageQueueContext   version:  1.0   ¡¤  date: 2015/11/18
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_MESSAGEQUEUECONTEX_HPP
#define LV_NET_MESSAGEQUEUECONTEX_HPP

#include <lv/FrameWork/Net/Context.hpp>

namespace lv { namespace net {

	class MessageQueueContext : public Context
	{
		bool	preserve_order_;

		double	resend_time_;

	public:

		MessageQueueContext(BufferManagerPtr buf_manager, ServiceWrapper const & service_wrapper, 
			bool preserve_order)
			: Context(buf_manager, service_wrapper)
			, preserve_order_(preserve_order)
			, resend_time_(20.0)
		{
		}

		bool	preserve_order() const
		{
			return preserve_order_;
		}

		void	set_resend_time(double time)
		{
			resend_time_ = time;
		}

		double	resend_time() const
		{
			return resend_time_;
		}
	};

} }

#endif