#include <lv/FrameWork/Net/SteadyTimer.hpp>


namespace lv::net
{
	SteadyTimer::SteadyTimer(IOContextWrapper const & io_wrapper, duration_type const & duration, Callback callback)
		: io_wrapper_(io_wrapper)
		, timer_(std::make_shared<timer_type>(io_wrapper.io_context(), duration))
		, duration_(duration)
		, callback_(std::move(callback))
	{
		start_timer();
	}

	void SteadyTimer::start_timer()
	{
		auto handler = [weak_timer = WeakTimerPtr(timer_), this](boost::system::error_code const & error) {
			on_timer(weak_timer, error);
		};

		if (io_wrapper_.has_strand())
		{
			timer_->async_wait(io_wrapper_.strand().wrap(handler));
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
			timer->expires_at(timer->expires_at() + duration_);
			start_timer();

			callback_();
		}
	}

}