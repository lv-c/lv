// *********************************************************************
//  ScopeGuard   version:  1.0   ¡¤  date: 09/13/2008
//  --------------------------------------------------------------------
//  An efficient implementation of general scope guard.
//	Another way is using boost::shared_ptr which may cause memory
//	allocation on the heap (when the function object passed in is not 
//	small enough?)
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SCOPEGUARD_HPP
#define LV_SCOPEGUARD_HPP


namespace lv
{
	class ScopeGuard
	{
	protected:

		ScopeGuard(){}

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