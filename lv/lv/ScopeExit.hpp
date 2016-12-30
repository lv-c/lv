// *********************************************************************
//  ScopeExit   version:  1.0   ·  date: 09/13/2008
//  --------------------------------------------------------------------
//  2016/07/12 ：由 ScopeGuard 改为 ScopeExit, 使用 C++11 让它更易于使用
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SCOPEGUARD_HPP
#define LV_SCOPEGUARD_HPP

#include <functional>

#include <boost/preprocessor/cat.hpp>
#include <boost/noncopyable.hpp>


namespace lv
{
	class ScopeExit : boost::noncopyable
	{
		std::function<void()>	fun_;

	public:

		explicit ScopeExit(std::function<void()> fun)
			: fun_(std::move(fun))
		{
		}


		~ScopeExit()
		{
			fun_();
		}
	};
}

#define LV_SCOPE_EXIT(fun) lv::ScopeExit BOOST_PP_CAT(_LV_EXIT_, __LINE__) (fun)


#endif