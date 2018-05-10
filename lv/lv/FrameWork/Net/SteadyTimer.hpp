// *********************************************************************
//  SteadyTimer   version:  1.0   ¡¤  date: 2015/11/19
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/ServiceWrapper.hpp>

#include <boost/asio/steady_timer.hpp>

#include <functional>


namespace lv::net
{
	class SteadyTimer
	{
		ServiceWrapper	service_wrapper_;

		using timer_type = boost::asio::steady_timer;

		using TimerPtr = std::shared_ptr<timer_type>;
		using WeakTimerPtr = std::weak_ptr<timer_type>;

		TimerPtr	timer_;

		using duration_type = timer_type::duration;
		duration_type	duration_;

		using Callback = std::function<void()>;
		Callback	callback_;

	public:

		SteadyTimer(ServiceWrapper const & service_wrapper, duration_type const & duration, Callback callback);

	private:

		SteadyTimer();

		void	start();

		void	on_timer(WeakTimerPtr weak_timer, boost::system::error_code const & error);

	};

}