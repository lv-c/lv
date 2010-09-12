// *********************************************************************
//  Reg   version:  1.0   ¡¤  date: 08/21/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_REG_HPP
#define LV_LUABIND_REG_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>


namespace lv { namespace lua {

	namespace detail
	{
		template<typename ret, typename wrap>
		typename boost::enable_if<boost::is_same<ret, void>, ret>::type return_value(wrap v)
		{
		}

		template<typename ret, typename wrap>
		typename boost::disable_if<boost::is_same<ret, void>, ret>::type return_value(wrap v)
		{
			return v;
		}
	}
} }



#define LUA_WRAPPER_FUN_0(base, ret, fun)	\
	virtual ret	fun()	\
	{	\
		return lv::lua::detail::return_value<ret>(call<ret>(#fun));	\
	}	\
		static ret default_##fun(base * ptr)	\
	{	\
		return ptr->base##::##fun();	\
	}	


#define LUA_WRAPPER_FUN_1(base, ret, fun, t0)	\
	virtual ret	fun(t0 v0)	\
	{	\
		return lv::lua::detail::return_value<ret>(call<ret>(#fun, v0));	\
	}	\
		static ret default_##fun(base * ptr, t0 v0)	\
	{	\
		return ptr->base##::##fun(v0);	\
	}

#define LUA_WRAPPER_FUN_2(base, ret, fun, t0, t1)	\
	virtual ret	fun(t0 v0, t1 v1)	\
	{	\
		return lv::lua::detail::return_value<ret>(call<ret>(#fun, v0, v1));	\
	}	\
		static ret default_##fun(base * ptr, t0 v0, t1 v1)	\
	{	\
		return ptr->base##::##fun(v0, v1);	\
	}	

#define LUA_WRAPPER_FUN_3(base, ret, fun, t0, t1, t2)	\
	virtual ret	fun(t0 v0, t1 v1, t2 v2)	\
	{	\
		return lv::lua::detail::return_value<ret>(call<ret>(#fun, v0, v1, v2));	\
	}	\
		static ret default_##fun(base * ptr, t0 v0, t1 v1, t2 v2)	\
	{	\
		return ptr->base##::##fun(v0, v1, v2);	\
	}	

#define LUA_REG_VIRTUAL_FUN(base, fun)	\
	.def(#fun, & base##::##fun, & base##Wrapper::default_##fun)

#define LUA_REG_FUN(base, fun)	\
	.def(#fun, & base##::##fun)

#endif
