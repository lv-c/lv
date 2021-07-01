// *********************************************************************
//  ScopeExit   version:  1.0   ·  date: 09/13/2008
//  --------------------------------------------------------------------
//  2016/07/12 ：由 ScopeGuard 改为 ScopeExit, 使用 C++11 让它更易于使用
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/preprocessor/cat.hpp>
#include <boost/noncopyable.hpp>


namespace lv
{
	template<class Fn>
	class ScopeExit : boost::noncopyable
	{
		Fn		fn_;

	public:

		explicit ScopeExit(Fn fn)
			: fn_(std::move(fn))
		{
		}

		~ScopeExit()
		{
			fn_();
		}
	};


	namespace detail
	{
		template<class Fn>
		ScopeExit<Fn>	make_scope_exit(Fn && fn)
		{
			return ScopeExit<Fn>(std::forward<Fn>(fn));
		}
	}
}

#define LV_SCOPE_EXIT(...) auto BOOST_PP_CAT(_LV_EXIT_, __LINE__) = lv::detail::make_scope_exit(__VA_ARGS__)
