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

#include <type_traits>


namespace lv::lua
{
	namespace detail
	{
		template<typename ret, typename wrap>
		std::enable_if_t<std::is_same<ret, void>::value, ret>	return_value(wrap v)
		{
		}

		template<typename ret, typename wrap>
		std::enable_if_t<! std::is_same<ret, void>::value, ret>	return_value(wrap v)
		{
			return v;
		}
	}
}



#define LUA_WRAPPER_FN_0(base, ret, fn)	\
	virtual ret	fn()	\
	{	\
		return lv::lua::detail::return_value<ret>(call<ret>(#fn));	\
	}	\
		static ret default_##fn(base * ptr)	\
	{	\
		return ptr->base::fn();	\
	}	


#define LUA_WRAPPER_FN_1(base, ret, fn, t0)	\
	virtual ret	fn(t0 v0)	\
	{	\
		return lv::lua::detail::return_value<ret>(call<ret>(#fn, v0));	\
	}	\
		static ret default_##fn(base * ptr, t0 v0)	\
	{	\
		return ptr->base::fn(v0);	\
	}

#define LUA_WRAPPER_FN_2(base, ret, fn, t0, t1)	\
	virtual ret	fn(t0 v0, t1 v1)	\
	{	\
		return lv::lua::detail::return_value<ret>(call<ret>(#fn, v0, v1));	\
	}	\
		static ret default_##fn(base * ptr, t0 v0, t1 v1)	\
	{	\
		return ptr->base::fn(v0, v1);	\
	}	

#define LUA_WRAPPER_FN_3(base, ret, fn, t0, t1, t2)	\
	virtual ret	fn(t0 v0, t1 v1, t2 v2)	\
	{	\
		return lv::lua::detail::return_value<ret>(call<ret>(#fn, v0, v1, v2));	\
	}	\
		static ret default_##fn(base * ptr, t0 v0, t1 v1, t2 v2)	\
	{	\
		return ptr->base::fn(v0, v1, v2);	\
	}	

#define LUA_REG_VIRTUAL_FN(base, fn)	\
	.def(#fn, & base::fn, & base##Wrapper::default_##fn)

#define LUA_REG_FN(base, fn)	\
	.def(#fn, & base::fn)

#endif
