// *********************************************************************
//  SteadyTimer   version:  1.0   ��  date: 2015/11/19
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_DEADLINETIMER_HPP
#define LV_NET_DEADLINETIMER_HPP

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/ServiceWrapper.hpp>

#include <boost/asio/steady_timer.hpp>

#include <functional>


namespace lv::net
{
	class SteadyTimer
	{
		ServiceWrapper	service_wrapper_;

		typedef boost::asio::steady_timer	timer_type;

		typedef std::shared_ptr<timer_type>	TimerPtr;
		typedef std::weak_ptr<timer_type>	WeakTimerPtr;

		TimerPtr	timer_;

		typedef timer_type::duration	duration_type;
		duration_type	duration_;

		typedef std::function<void()>		Callback;
		Callback	callback_;

	public:

		SteadyTimer(ServiceWrapper const & service_wrapper, duration_type const & duration,
			Callback const & callback);

	private:

		SteadyTimer();

		void	start();

		void	on_timer(WeakTimerPtr weak_timer, boost::system::error_code const & error);

	};

}

#endif