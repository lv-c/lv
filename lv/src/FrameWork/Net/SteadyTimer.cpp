#include <lv/FrameWork/Net/SteadyTimer.hpp>

#include <functional>


namespace lv { namespace net {

	SteadyTimer::SteadyTimer(ServiceWrapper const & service_wrapper, duration_type const & duration, Callback const & callback)
		: service_wrapper_(service_wrapper)
		, timer_(std::make_shared<timer_type>(service_wrapper.service()))
		, duration_(duration)
		, callback_(callback)
	{
		start();
	}

	void SteadyTimer::start()
	{
		timer_->expires_from_now(duration_);

		if (service_wrapper_.has_strand())
		{
			timer_->async_wait(service_wrapper_.strand().wrap(std::bind(&SteadyTimer::on_timer, this, WeakTimerPtr(timer_), std::placeholders::_1)));
		}
		else
		{
			timer_->async_wait(std::bind(&SteadyTimer::on_timer, this, WeakTimerPtr(timer_), std::placeholders::_1));
		}
	}

	void SteadyTimer::on_timer(WeakTimerPtr weak_timer, boost::system::error_code const & error)
	{
		if (error)
		{
			return;
		}

		if (TimerPtr timer = weak_timer.lock())
		{
			start();
			callback_();
		}
	}

} }