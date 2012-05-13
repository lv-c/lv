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

namespace lv
{

	// How to use: ScopeGuard const & guard = scope_guard(boost::bind<void>(&ReleaseDC, hWnd, hDC));

	class ScopeGuard
	{
	protected:

		bool	enabled_;

	protected:

		ScopeGuard() : enabled_(true) {}

		/**
		 * This's made protected to ensure that the user will not hold an object of the @a ScopeGuard type
		 * rather than a reference to an object of the @a detail::ScopeGuarDerived type by mistake.
		 */
		ScopeGuard(ScopeGuard const &){}

	public:

		void	enable(bool e)
		{
			enabled_ = e;
		}

		bool	enabled() const
		{
			return enabled_;
		}

		virtual ~ ScopeGuard(){}
	};

	namespace detail
	{
		template<class T>
		class ScopeGuarDerived : public ScopeGuard
		{
			T t_;

		public:

			ScopeGuarDerived(T const & t)
				: t_(t)
			{
			}

			virtual	~ScopeGuarDerived()
			{
				if(enabled_)
				{
					t_();
				}
			}
		};
	}

	template<class T>
	detail::ScopeGuarDerived<T> scope_guard(T t)
	{
		return t;
	}

}

#endif // LV_SCOPEGUARD_HPP