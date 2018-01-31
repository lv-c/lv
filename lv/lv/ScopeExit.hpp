// *********************************************************************
//  ScopeExit   version:  1.0   ��  date: 09/13/2008
//  --------------------------------------------------------------------
//  2016/07/12 ���� ScopeGuard ��Ϊ ScopeExit, ʹ�� C++11 ����������ʹ��
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <functional>

#include <boost/preprocessor/cat.hpp>
#include <boost/noncopyable.hpp>


namespace lv
{
	class ScopeExit : boost::noncopyable
	{
		std::function<void()>	fn_;

	public:

		explicit ScopeExit(std::function<void()> fn)
			: fn_(std::move(fn))
		{
		}


		~ScopeExit()
		{
			fn_();
		}
	};
}

#define LV_SCOPE_EXIT(fn) lv::ScopeExit BOOST_PP_CAT(_LV_EXIT_, __LINE__) (fn)
