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
				cps_ = static_cast<uint64>(frequency.QuadPart);
#else
				cps_ = CLOCKS_PER_SEC;
#endif
			}

			restart();
		}


		inline void	restart() 
		{
			start_time_ = cur_time();
		}

		
		// return the seconds that has elapsed
		inline double	elapsed() const
		{
			return cur_time() - start_time_;
		}

	private:

		inline double	cur_time() const			// return the current time
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

		double	start_time_;		

		static	uint64	cps_;
	};


}


#endif	// LV_TIMER_HPP