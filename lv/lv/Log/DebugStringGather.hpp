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

#include <lv/Log/StringGather.hpp>

namespace lv
{
	class DebugStringGather : public StringGather
	{
		struct Receiver
		{
			void operator () (std::string const & str, log::level) const
			{
				::OutputDebugStringA(str.c_str());
			}
		};

	public:

		explicit DebugStringGather(filter_t filter = filter_t())
			: StringGather(Receiver(), filter)
		{
		}
	};
}

#endif // LV_PLATFORM_WINDOWS

#endif // LV_DEBUGSTRINGGATHER_HPP