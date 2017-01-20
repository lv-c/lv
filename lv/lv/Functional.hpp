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

#ifndef LV_FUNCTIONAL_HPP
#define LV_FUNCTIONAL_HPP

#include <lv/detail/RemoveSecond.hpp>

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/components.hpp>

#include <type_traits>


namespace lv
{
	namespace detail
	{
		namespace ft = boost::function_types;

		template<typename T, typename ClassTypeTransform, class Enable = void>
		struct SignatureImpl;

		// function
		template<typename T, typename ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, std::enable_if_t<std::is_function<T>::value> >
		{
			typedef T	type;
		};


		namespace mpl = boost::mpl;

		// member function
		template<typename T, typename ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, std::enable_if_t<std::is_member_function_pointer<T>::value> >
		{
			typedef typename ft::function_type<typename ft::components<T, ClassTypeTransform>::type>::type type;
		};


		// class type function object
		template<typename T, typename ClassTypeTransform>
		struct SignatureImpl<T, ClassTypeTransform, std::enable_if_t<std::is_class<T>::value> >
		{
			typedef typename ft::function_type<
				typename detail::RemoveSecond<
					typename ft::components<
						decltype(&T::operator ())
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
		std::function<typename SignatureImpl<F>::type> fn = f;
	}
	*/


	/**
	 * @param ClassTypeTransform see the document of boost::function_types::components. It's useful only when
	 *	T is a member function pointer type
	 */
	template<typename T, typename ClassTypeTransform = boost::add_reference<boost::mpl::_> >
	struct Signature : detail::SignatureImpl<std::remove_pointer_t<T>, ClassTypeTransform>
	{
	};

}

#endif
