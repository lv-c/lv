// *********************************************************************
//  DeadlineTimer   version:  1.0   ¡¤  date: 2015/11/19
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_DEADLINETIMER_HPP
#define LV_NET_DEADLINETIMER_HPP

#include <lv/ServiceWrapper.hpp>

#include <boost/asio/deadline_timer.hpp>

#include <functional>


namespace lv { namespace net {

	class DeadlineTimer
	{
		ServiceWrapper	service_wrapper_;

		typedef std::shared_ptr<boost::asio::deadline_timer>	TimerPtr;
		typedef std::weak_ptr<boost::asio::deadline_timer>	WeakTimerPtr;

		TimerPtr	timer_;

		boost::posix_time::time_duration	duration_;

		typedef std::function<void()>		Callback;
		Callback	callback_;

	public:

		DeadlineTimer(ServiceWrapper const & service_wrapper, boost::posix_time::time_duration const & duration,
			Callback const & callback);

	private:

		DeadlineTimer();

		void	start();

		void	on_timer(WeakTimerPtr weak_timer, boost::system::error_code const & error);

	};

} }

#endif