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

#include <lv/lvlib2.hpp>
#include <lv/IntType.hpp>

#include <boost/operators.hpp>

namespace lv
{
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

	class Timer
	{
		double	start_time_;		

		static	uint64	cps_;

	public:
		// start the timer
		Timer();

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

		inline double	cur_time() const;			// return the current time
	};
}


#endif
