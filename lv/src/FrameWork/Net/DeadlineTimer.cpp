#include <lv/FrameWork/Net/DeadlineTimer.hpp>

#include <boost/bind.hpp>

namespace lv { namespace net {

	DeadlineTimer::DeadlineTimer(ServiceWrapper const & service_wrapper, boost::posix_time::time_duration const & duration, Callback const & callback)
		: service_wrapper_(service_wrapper)
		, timer_(new boost::asio::deadline_timer(service_wrapper.service()))
		, duration_(duration)
		, callback_(callback)
	{
		start();
	}

	void DeadlineTimer::start()
	{
		timer_->expires_from_now(duration_);

		if (service_wrapper_.has_strand())
		{
			timer_->async_wait(service_wrapper_.strand().wrap(boost::bind(&DeadlineTimer::on_timer, this, WeakTimerPtr(timer_), _1)));
		}
		else
		{
			timer_->async_wait(boost::bind(&DeadlineTimer::on_timer, this, WeakTimerPtr(timer_), _1));
		}
	}

	void DeadlineTimer::on_timer(WeakTimerPtr weak_timer, boost::system::error_code const & error)
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