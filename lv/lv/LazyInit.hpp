// *********************************************************************
//  LazyInit   version:  1.0   ¡¤  date: 2015/11/23
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LAZYINIT_HPP
#define LV_LAZYINIT_HPP

#include <boost/optional/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

#include <mutex>


namespace lv
{
	// thread-safe lazy initialization

	template<class T, class Expr = boost::in_place_factory0>
	class LazyInit
	{
		typedef T	value_type;

		boost::optional<value_type>	value_;

		// from optional document

		// Creates an optional<T> with an expression which can be either
		//  (a) An instance of InPlaceFactory (i.e. in_place(a,b,...,n);
		//  (b) An instance of TypedInPlaceFactory ( i.e. in_place<T>(a,b,...,n);
		//  (c) Any expression implicitely convertible to the single type
		//      of a one-argument T's constructor.
		//  (d*) Weak compilers (BCB) might also resolved Expr as optional<T> and optional<U>
		//       even though explicit overloads are present for these.
		// Depending on the above some T ctor is called.

		Expr			expr_;

		std::once_flag	once_;

	public:

		LazyInit(Expr const & expr = Expr())
			: expr_(expr)
		{
		}

		value_type &	get()
		{
			std::call_once(once_, [this] { value_ = expr_; });
			return *value_;
		}
	};
}

#endif
