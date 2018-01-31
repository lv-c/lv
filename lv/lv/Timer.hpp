// *********************************************************************
//  Timer   version:  1.0   ¡¤  date: 11/12/2007
//  --------------------------------------------------------------------
//					high precision timer
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/operators.hpp>

#include <chrono>


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
		using clock_type = std::chrono::high_resolution_clock;

		clock_type::time_point	start_time_;

	public:

		Timer()
		{
			restart();
		}

		inline void	restart() 
		{
			start_time_ = clock_type::now();
		}
		
		inline TimeSpan	elapsed() const
		{
			clock_type::duration span = clock_type::now() - start_time_;
			return TimeSpan(static_cast<double>(span.count()) * clock_type::period::num / clock_type::period::den);
		}
	};
}

