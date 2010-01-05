// *********************************************************************
//  MemFn   version:  1.0   ��  date: 12/24/2009
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

#include <lv/detail/RemoveSecond.hpp>

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/components.hpp>

#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace lv
{
	template<typename MemFn>
	struct BindMemFnSignature
	{
		BOOST_MPL_ASSERT((boost::is_member_function_pointer<MemFn>));

		typedef typename boost::function_types::function_type<
			typename detail::RemoveSecond<
				typename boost::function_types::components<MemFn>::type
			>::type 
		>::type type;
	};

	namespace result_of
	{
		template<typename MemFn>
		struct bind_mem_fn
		{
			typedef boost::function<typename BindMemFnSignature<MemFn>::type>	type;
		};
	}

#	ifndef LV_MEMFUN_MAX_ARITY
#	define LV_MEMFUN_MAX_ARITY	5
#	endif
	
	// bind_mem_fn
#	define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_MEMFUN_MAX_ARITY, <lv/MemFn.hpp>))
#	include BOOST_PP_ITERATE()

}


#endif // LV_MEMFN_HPP

#else

#define n BOOST_PP_ITERATION()

template<typename R, typename T, typename T0 BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>
boost::function<R(BOOST_PP_ENUM_PARAMS(n, typename A))> bind_mem_fn(R (T::*f)(BOOST_PP_ENUM_PARAMS(n, A)), T0 t)
{
	return boost::bind(f, t BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(n), _));
}

// const version
template<typename R, typename T, typename T0 BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>
boost::function<R(BOOST_PP_ENUM_PARAMS(n, typename A))> bind_mem_fn(R (T::*f)(BOOST_PP_ENUM_PARAMS(n, A)) const, T0 t)
{
	return boost::bind(f, t BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(n), _));
}


#undef n

#endif