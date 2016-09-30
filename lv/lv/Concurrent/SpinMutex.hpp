// *********************************************************************
//  SpinMutex   version:  1.0   ¡¤  date: 2016/09/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2016 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_SPINMUTEX_HPP
#define LV_CONCURRENT_SPINMUTEX_HPP

#include <atomic>


namespace lv
{
	// " It should be used only for locking short critical sections (typically less than 20 instructions) when fairness is not an issue." (Intel.TBB)

	class SpinMutex
	{
		std::atomic_flag	flag_ = ATOMIC_FLAG_INIT;

	public:

		void	lock()
		{
			while (flag_.test_and_set(std::memory_order_acquire)) {}
		}

		void	unlock()
		{
			flag_.clear(std::memory_order_release);
		}
	};
}

#endif