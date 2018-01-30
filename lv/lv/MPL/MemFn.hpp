// *********************************************************************
//  MemFn   version:  1.0   ¡¤  date: 12/24/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#if !BOOST_PP_IS_ITERATING

#ifndef LV_MEMFN_HPP
#define LV_MEMFN_HPP

#include <lv/MPL/detail/RemoveSecond.hpp>

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/components.hpp>

#include <functional>


namespace lv
{
	template<class MemFn>
	struct BindMemFnSignature
	{
		static_assert(std::is_member_function_pointer_v<MemFn>);

		using type = typename boost::function_types::function_type<
			typename detail::RemoveSecond<
				typename boost::function_types::components<MemFn>::type
			>::type 
		>::type;
	};

	namespace result_of
	{
		template<class MemFn>
		struct bind_mem_fn
		{
			using type = std::function<typename BindMemFnSignature<MemFn>::type>;
		};
	}

#	ifndef LV_MEMFUN_MAX_ARITY
#	define LV_MEMFUN_MAX_ARITY	5
#	endif
	
	// bind_mem_fn
#	define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_MEMFUN_MAX_ARITY, <lv/MPL/MemFn.hpp>))
#	include BOOST_PP_ITERATE()

}


#endif

#else

#define n BOOST_PP_ITERATION()

template<class R, class T, class T0 BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>
std::function<R(BOOST_PP_ENUM_PARAMS(n, A))> bind_mem_fn(R (T::*f)(BOOST_PP_ENUM_PARAMS(n, A)), T0 t)
{
	return std::bind(f, t BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(n), std::placeholders::_));
}

// const version
template<class R, class T, class T0 BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class A)>
std::function<R(BOOST_PP_ENUM_PARAMS(n, A))> bind_mem_fn(R (T::*f)(BOOST_PP_ENUM_PARAMS(n, A)) const, T0 t)
{
	return std::bind(f, t BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(n), std::placeholders::_));
}


#undef n

#endif
