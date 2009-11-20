// *********************************************************************
//  Ref   version:  1.0   ¡¤  date: 12/03/2008
//  --------------------------------------------------------------------
//  A reference wrapper which has a default constructor......
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_REF_HPP
#define LV_REF_HPP

#include <boost/mpl/if.hpp>
#include <boost/ref.hpp>

namespace lv
{

	template<typename T>
	class RefWrapper
	{
		T * t_;
	public:

		typedef T type;

		// here is the difference between this class and boost::reference_wrapper
		RefWrapper() : t_(0) {}

		explicit RefWrapper(T& t): t_(boost::addressof(t)) {}

		RefWrapper(boost::reference_wrapper<T> const & r) : t_(r.get_pointer()) {}
		RefWrapper const & operator = (boost::reference_wrapper<T> const & r) 
		{
			t_ = r.get_pointer();
			return *this;
		}

		operator T& () const 
		{ 
			return *t_; 
		}

		T& get() const 
		{ 
			return *t_; 
		}

		T* get_pointer() const 
		{ 
			return t_; 
		}

	};


	/// if T is a reference type, use RefWrapper instead
	/*
	template<typename T>
	struct ChangeRefWrapper
	{
		typedef typename boost::mpl::if_<
			boost::is_reference_wrapper<T>,
			RefWrapper<typename boost::unwrap_reference<T>::type>,
			T
		>::type type;
	};
	*/
	
}

namespace boost
{
	template<typename T>
	class is_reference_wrapper<lv::RefWrapper<T> >
		: public mpl::true_
	{
	};

	template<typename T>
	class unwrap_reference<lv::RefWrapper<T> >
	{
	public:
		typedef T type;
	};
}

#endif // LV_REF_HPP
