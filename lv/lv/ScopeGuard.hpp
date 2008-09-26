// *********************************************************************
//  ScopeGuard   version:  1.0   ¡¤  date: 09/13/2008
//  --------------------------------------------------------------------
//  A zero overhead implementation of general scope guard.
//	Another way is using boost::shared_ptr which may lead to memory
//	allocation on the heap
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SCOPEGUARD_HPP
#define LV_SCOPEGUARD_HPP

#include <boost/scoped_ptr.hpp>
namespace lv
{

	// How to use: ScopeGuard const & guard = scope_guard(boost::bind<void>(&ReleaseDC, hWnd, hDC);

	class ScopeGuard
	{
	protected:

		ScopeGuard(){}

		/**
		 * This's made protected to ensure that the user will not hold an object of the @a ScopeGuard type
		 * rather than a reference to an object of the @a detail::ScopeGuarDerived type by mistake.
		 */
		ScopeGuard(ScopeGuard const &){}

	public:

		virtual ~ ScopeGuard(){}
	};

	namespace detail
	{
		template<class T>
		class ScopeGuarDerived :ScopeGuard
		{
			T t_;

		public:

			ScopeGuarDerived(T const & t)
				: t_(t)
			{
			}

			virtual	~ScopeGuarDerived()
			{
				t_();
			}
		}
	}

	template<class T>
	detail::ScopeGuarDerived<T> scope_guard(T t)
	{
		return t;
	}

}

#endif // LV_SCOPEGUARD_HPP