// *********************************************************************
//  DelayTimer   version:  1.0   ¡¤  date: 02/14/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DELAYTIMER_HPP
#define LV_DELAYTIMER_HPP

#include <lv/Config.hpp>

#include <boost/assert.hpp>

#ifdef LV_PLATFORM_WINDOWS
#	include <Windows.h>

namespace lv
{
	class DelayTimer
	{
		HANDLE timer_;

	public:

		explicit DelayTimer(char const * name = "LvDelayTimer")
		{
			timer_ = CreateWaitableTimerA(NULL, TRUE, name);
			BOOST_ASSERT(timer_ != NULL);
		}

		~DelayTimer()
		{
			CloseHandle(timer_);
			timer_ = NULL;
		}

		void	sleep(int ms) const
		{
			LARGE_INTEGER due_time;
			due_time.QuadPart = - ms * 10000;

			if(timer_ != NULL && SetWaitableTimer(timer_, &due_time, 0, NULL, NULL, 0))
			{
				if(WaitForSingleObject(timer_, INFINITE) == WAIT_OBJECT_0)
				{
					return;
				}
			}

			// failed. use Sleep instead
			::Sleep(ms);

			BOOST_ASSERT(false);
		}

	};
}


#else
#	include <unistd.h>

namespace lv
{
	class DelayTimer
	{
	public:

		explicit DelayTimer(char const * dummy_name = "LvDelayTimer") {}

		~DelayTimer() {}

		void sleep(int ms) const
		{
			BOOST_ASSERT(ms < 1000000);
			usleep(ms);
		}
	};
}

#endif

#endif