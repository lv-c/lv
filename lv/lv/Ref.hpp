// *********************************************************************
//  Ref   version:  1.0   ��  date: 12/03/2008
//  --------------------------------------------------------------------
//  A reference wrapper which has a default constructor......
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <type_traits>


namespace lv
{

	template<class T>
	class RefWrapper
	{
		T * t_;

	public:

		using type = T;

		// here is the difference between this class and std::reference_wrapper
		RefWrapper() : t_(nullptr) {}

		explicit RefWrapper(T & t): t_(std::addressof(t)) {}

		RefWrapper(std::reference_wrapper<T> const & r) : t_(&r.get()) {}

		RefWrapper const & operator = (std::reference_wrapper<T> const & r) 
		{
			t_ = &r.get();
			return *this;
		}

		operator T & () const 
		{ 
			return *t_; 
		}

		T &	get() const 
		{ 
			return *t_; 
		}

	};
		
}
