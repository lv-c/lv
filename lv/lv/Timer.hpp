// *********************************************************************
//  Timer   version:  1.0   ¡¤  date: 11/12/2007
//  --------------------------------------------------------------------
//					high precision timer
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_TIMER_HPP
#define LV_TIMER_HPP

#include <lv/Config.hpp>

#include <boost/operators.hpp>

#ifdef LV_PLATFORM_WINDOWS
#	include <Windows.h>
#else
#	include <ctime>
#endif

namespace lv
{
	/// fwd.  You should use Timer rather than TimerT
	template<class>	class TimerT;
	typedef TimerT<void> Timer;

	class TimeSpan : public boost::additive<TimeSpan>
	{

		double	seconds_;

	public:

		// seconds
		explicit TimeSpan(double sec)
			: seconds_(sec)
		{
		}

		// seconds
		inline operator double() const
		{
			return seconds_;
		}

		inline double	milliseconds() const
		{
			return seconds() * 1000.0;
		}

		inline double	seconds() const
		{
			return seconds_;
		}

		inline double	minutes() const
		{
			return seconds() / 60.0;
		}

		inline double	hours() const
		{
			return minutes() / 60.0;
		}

		inline double	days() const
		{
			return hours() / 24.0;
		}


		TimeSpan & operator += (TimeSpan const & rhs)
		{
			seconds_ += rhs.seconds_;
			return *this;
		}

		TimeSpan & operator -= (TimeSpan const & rhs)
		{
			seconds_ -= rhs.seconds_;
			return *this;
		}
		
	};


	/**
	 * use template to make it a header-only class
	 */
	template<class Dummy = void>	
	class TimerT
	{
	public:
		// start the timer
		TimerT()
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
		inline TimeSpan	elapsed() const
		{
			return TimeSpan(cur_time() - start_time_);
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

	template<class T>
	uint64 TimerT<T>::cps_ = 0;

}


#endif
