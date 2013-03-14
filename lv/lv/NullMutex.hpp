// *********************************************************************
//  NullMutex   version:  1.0   ¡¤  date: 08/20/2008
//  --------------------------------------------------------------------
//			a null mutex.
//	see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2043.html
// "Simplifying And Extending Mutex and Scoped Lock Types For C++ 
//		Multi-Threading Library"
//	
//	In fact, the overhead of boost::mutex is really negligible and there's
//	no need to use this in general.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NULLMUTEX_HPP
#define LV_NULLMUTEX_HPP

#include <boost/noncopyable.hpp>
#include <boost/thread/locks.hpp>

namespace lv
{
	class NullMutex : boost::noncopyable
	{
	public:

		typedef boost::unique_lock<NullMutex>	scoped_lock;

		NullMutex()
		{
		}

		~NullMutex()
		{
		}

		void	lock()
		{
		}

		void	unlock()
		{
		}

		bool try_lock()
		{
			return true;
		}

		bool timed_lock(boost::system_time const & abs_time)
		{
			return true;
		}

		template <typename TimeDuration>
		bool timed_lock(TimeDuration const & relative_time)
		{
			return true;
		}
	};
}

#endif