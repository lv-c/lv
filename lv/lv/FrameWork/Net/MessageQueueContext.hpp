// *********************************************************************
//  MessageQueueContext   version:  1.0   ��  date: 2015/11/18
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/Context.hpp>


namespace lv::net
{
	class MessageQueueContext : public Context
	{
		bool	preserve_order_;

		double	resend_time_;

	public:

		MessageQueueContext(IOContextWrapper const & io_wrapper, bool preserve_order)
			: Context(io_wrapper)
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

}