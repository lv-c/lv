// *********************************************************************
//  Functional   version:  1.0   ¡¤  date: 11/25/2008
//  --------------------------------------------------------------------
//  deduce the Signature of all function objects that can be used with 
//	the std::function template
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/MPL/detail/RemoveSecond.hpp>

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/components.hpp>

#include <type_traits>


namespace lv
{
	namespace detail
	{
		namespace ft = boost::function_types;

		template<class T, class ClassTypeTransform, class Enable = void>
		struct SignatureImpl;

		// function
		template<class T, class ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, std::enable_if_t<std::is_function_v<T> > >
		{
			using type = T;
		};


		namespace mpl = boost::mpl;

		// member function
		template<class T, class ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, std::enable_if_t<std::is_member_function_pointer_v<T> > >
		{
			using type = typename ft::function_type<typename ft::components<T, ClassTypeTransform>::type>::type;
		};


		// class type function object
		template<class T, class ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, std::enable_if_t<std::is_class_v<T> > >
		{
			using type = typename ft::function_type<
				typename detail::RemoveSecond<
					typename ft::components<
						decltype(&T::operator ())
					>::type
				>::type
			>::type;
		};
	}

	/*
	// usage :
	template<class F>
	void reg(F f)	// F can be a function type, a member function pointer type or a class type function object type
	{
		std::function<typename SignatureImpl<F>::type> fn = f;
	}
	*/


	/**
	 * @param ClassTypeTransform see the document of boost::function_types::components. It's useful only when
	 *	T is a member function pointer type
	 */
	template<class T, class ClassTypeTransform = boost::add_reference<boost::mpl::_> >
	struct Signature : detail::SignatureImpl<std::remove_pointer_t<T>, ClassTypeTransform>
	{
	};

}
