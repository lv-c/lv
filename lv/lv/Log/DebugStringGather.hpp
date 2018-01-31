// *********************************************************************
//  DebugStringGather   version:  1.0   ¡¤  date: 11/30/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS

#include <Windows.h>

#include <lv/Log/Fwd.hpp>
#include <lv/Log/StringGather.hpp>


namespace lv::log
{
	class DebugStringGather : public StringGather
	{
		struct Receiver
		{
			void operator () (string_type && str, int) const
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
}

#endif