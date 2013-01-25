// *********************************************************************
//  UnitTest   version:  1.0   ·  date: 11/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#define _SCL_SECURE_NO_WARNINGS

// debug 版下用 vld 检测内存。 使用了 quick allocator 最后的内存没被释放
#ifndef _DEBUG
#define BOOST_SP_USE_QUICK_ALLOCATOR
#endif

#include <boost/test/unit_test.hpp>