// *********************************************************************
//  DebugStringGather   version:  1.0   ¡¤  date: 11/30/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DEBUGSTRINGGATHER_HPP
#define LV_DEBUGSTRINGGATHER_HPP

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS

#include <Windows.h>

#include <lv/Log/Fwd.hpp>
#include <lv/Log/StringGather.hpp>

namespace lv { namespace log {
	
	class DebugStringGather : public StringGather
	{
		struct Receiver
		{
			void operator () (string_type const & str, int) const
			{
#ifndef LV_UNICODE_LOG
				::OutputDebugStringA(str.c_str());
#else
				::OutputDebugStringW(str.c_str());
#endif
			}
		};

	public:

		explicit DebugStringGather(filter_type filter = filter_type())
			: StringGather(Receiver(), filter)
		{
		}
	};
} }

#endif // LV_PLATFORM_WINDOWS

#endif // LV_DEBUGSTRINGGATHER_HPP