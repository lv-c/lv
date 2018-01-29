// *********************************************************************
//  UnitTest   version:  1.0   ��  date: 11/23/2008
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

#	define _SCL_SECURE_NO_WARNINGS
#	define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
#	define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING

#endif

// debug ������ vld ����ڴ档 ʹ���� quick allocator �����ڴ�û���ͷ�
#ifndef _DEBUG
#define BOOST_SP_USE_QUICK_ALLOCATOR
#endif

#include <boost/test/unit_test.hpp>