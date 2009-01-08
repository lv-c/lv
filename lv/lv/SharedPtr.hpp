// *********************************************************************
//  SharedPtr   version:  1.0   ¡¤  date: 11/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#if !BOOST_PP_IS_ITERATING

#ifndef LV_SHAREDPTR_HPP
#define LV_SHAREDPTR_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include <lv/Config.hpp>

#include <boost/shared_ptr.hpp>


#ifndef LV_SHARED_NEW_MAX_PARAMS_NUM
#define LV_SHARED_NEW_MAX_PARAMS_NUM	8
#endif

namespace lv
{
	/// NullDeleter
	struct NullDeleter
	{
		void	operator() (void const *) const 
		{
		}
	};


	/// shared_from_object
	template<class T>
	boost::shared_ptr<T> shared_from_object(T & t)
	{
		return boost::shared_ptr<T>(&t, NullDeleter());
	}

	/// shared_new<T>(...parameters for constructor of T...)
	/// parameters are passed as T const & type, if you want a T & type, use boost::ref
#	define SMART_POINTER_TYPE boost::shared_ptr
#	define SHARED_NEW_FUNC_NAME	shared_new
#	define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_SHARED_NEW_MAX_PARAMS_NUM, <lv/SharedPtr.hpp>))
#	include BOOST_PP_ITERATE()


	// auto_new<T>(...parameters for constructor of T...)
#	define SMART_POINTER_TYPE std::auto_ptr
#	define SHARED_NEW_FUNC_NAME	auto_new
#	define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_SHARED_NEW_MAX_PARAMS_NUM, <lv/SharedPtr.hpp>))
#	include BOOST_PP_ITERATE()

#ifdef LV_PLATFORM_WINDOWS


#endif // LV_PLATFORM_WINDOWS

}


#endif // LV_SHAREDPTR_HPP

#else

#if !defined SMART_POINTER_TYPE || !defined SHARED_NEW_FUNC_NAME
#error define SMART_POINTER_TYPE and SHARED_NEW_FUNC_NAME first
#endif

#define LV_SHARED_NEW_ctor_params(z, n, data) T##n const & t##n

#define n BOOST_PP_ITERATION()


template <class T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class T) >
BOOST_PP_CAT(SMART_POINTER_TYPE, <T>) SHARED_NEW_FUNC_NAME(BOOST_PP_ENUM(n, LV_SHARED_NEW_ctor_params , ~))
{
	return BOOST_PP_CAT(SMART_POINTER_TYPE, <T>)(new T(BOOST_PP_ENUM_PARAMS(n, t)));
}

#if n == LV_SHARED_NEW_MAX_PARAMS_NUM	// the last iteration
#undef SMART_POINTER_TYPE
#undef SHARED_NEW_FUNC_NAME
#endif

#undef LV_SHARED_NEW_ctor_params
#undef n

#endif