// *********************************************************************
//  ComPtr   version:  1.0   ¡¤  date: 11/20/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_COMPTR_HPP
#define LV_COMPTR_HPP

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS

#define BOOST_MEM_FN_ENABLE_STDCALL
#include <boost/mem_fn.hpp>
#include <boost/shared_ptr.hpp>

namespace lv
{
	template<typename T>
	inline boost::shared_ptr<T>	com_ptr(T * p)
	{
		return boost::shared_ptr<T>(p, boost::mem_fn(&T::Release));
	}
}

#endif // LV_PLATFORM_WINDOWS

#endif // LV_COMPTR_HPP