// *********************************************************************
//  Functional   version:  1.0   ¡¤  date: 11/25/2008
//  --------------------------------------------------------------------
//  deduce the Signature of all function objects that can be used with 
//	the boost::function template
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FUNCTIONAL_HPP
#define LV_FUNCTIONAL_HPP

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/components.hpp>

#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/erase.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/typeof/typeof.hpp>

namespace lv
{
	namespace detail
	{
		namespace ft = boost::function_types;

		template<typename T, typename ClassTypeTransform, class Enable = void>
		struct SignatureImpl;

		// function
		template<typename T, typename ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, typename boost::enable_if<boost::is_function<T> >::type>
		{
			typedef T	type;
		};


		namespace mpl = boost::mpl;

		// member function
		template<typename T, typename ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, typename boost::enable_if<boost::is_member_function_pointer<T> >::type>
		{
			typedef typename ft::function_type<typename ft::components<T, ClassTypeTransform>::type>::type type;
		};


		template<class T>
		struct RemoveSecond
		{
			typedef typename mpl::erase<T, 
				typename mpl::next<
					typename mpl::begin<T>::type
				>::type
			>::type type;
		};

		// class type function object
		template<typename T, typename ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, typename boost::enable_if<boost::is_class<T> >::type>
		{
			typedef typename ft::function_type<
				typename detail::RemoveSecond<
					typename ft::components<
						typename BOOST_TYPEOF(&T::operator ())
					>::type
				>::type
			>::type type;
		};
	}

	/*
	// usage :
	template<typename F>
	void reg(F f)	// F can be a function type, a member function pointer type or a class type function object type
	{
		boost::function<typename SignatureImpl<F>::type> fun = f;
	}
	*/


	/**
	 * @param ClassTypeTransform see the document of boost::function_types::components. It's useful only when
	 *	T is a member function pointer type
	 */
	template<typename T, typename ClassTypeTransform = boost::add_reference<boost::mpl::_> >
	struct Signature : detail::SignatureImpl<typename boost::remove_pointer<T>::type, ClassTypeTransform>
	{
	};

}

#endif // LV_FUNCTIONAL_HPP