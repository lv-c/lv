#include <lv/FrameWork/Net/SteadyTimer.hpp>


namespace lv::net
{
	SteadyTimer::SteadyTimer(ServiceWrapper const & service_wrapper, duration_type const & duration, Callback callback)
		: service_wrapper_(service_wrapper)
		, timer_(std::make_shared<timer_type>(service_wrapper.service()))
		, duration_(duration)
		, callback_(std::move(callback))
	{
		start();
	}

	void SteadyTimer::start()
	{
		timer_->expires_from_now(duration_);

		auto handler = [weak_timer = WeakTimerPtr(timer_), this](boost::system::error_code const & error) {
			on_timer(weak_timer, error);
		};

		if (service_wrapper_.has_strand())
		{
			timer_->async_wait(service_wrapper_.strand().wrap(handler));
		}
		else
		{
			timer_->async_wait(handler);
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

}