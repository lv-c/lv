// *********************************************************************
//  Timer   version:  1.0   ¡¤  date: 11/12/2007
//  --------------------------------------------------------------------
//						¸ß¾«¶È Timer
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_TIMER_HPP
#define LV_TIMER_HPP

#include <boost/assert.hpp>
#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS
#	include <Windows.h>
#else
#	include <ctime>
#endif

namespace lv
{
	class Timer
	{
	public:
		// start the timer
		Timer()
		{
			if(cps_ == 0)
			{
#ifdef LV_PLATFORM_WINDOWS
				LARGE_INTEGER frequency;
				QueryPerformanceFrequency(&frequency);
				cps_ = static_cast<uint64_t>(frequency.QuadPart);
#else
				cps_ = CLOCKS_PER_SEC;
#endif
			}

			restart();
		}

		Timer(bool paused)
			: paused_(paused)
		{
			if(paused)
				cumulate_time_ = 0.0;
			else
				restart();
		}

		void	restart() 
		{
			cumulate_time_ = 0.0;
			start_time_ = cur_time();
			paused_ = false;
		}

		void	pause()
		{
			BOOST_ASSERT(! paused_);

			if(! paused_)
			{
				cumulate_time_ += cur_time() - start_time_;
				paused_ = true;
			}
		}

		bool	paused() const
		{
			return paused_;
		}

		void	resume()
		{
			BOOST_ASSERT(paused_);

			if(paused_)
			{
				start_time_ = cur_time();
				paused_ = false;
			}
		}
		
		// return the seconds that has elapsed
		double	elapsed() const
		{
			if(paused_)
				return cumulate_time_;
			else
				return cumulate_time_ + cur_time() - start_time_;
		}
	private:

		double	cur_time() const			// return the current time
		{
#ifdef LV_PLATFORM_WINDOWS
			LARGE_INTEGER count;
			QueryPerformanceCounter(&count);
			return static_cast<double>(count.QuadPart) / cps_;
#else
			return static_cast<double>(std::clock()) / cps_;
#endif
		}
	private:
		double	cumulate_time_;
		double	start_time_;		// the time when 'restart' or 'resume' is called
		bool	paused_;

		static	uint64	cps_;
	};

	uint64 Timer::cps_ = 0;

}


#endif	// LV_TIMER_HPP