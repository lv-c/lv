// *********************************************************************
//  Log   version:  1.0   ¡¤  date: 05/05/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#define LV_UNICODE_LOG
#include <lv/Log/LogManager.hpp>
#include <lv/Log/Utility.hpp>

BOOST_AUTO_TEST_CASE(test_log)
{
	lv::Log & log = lv::LogManager::instance().create_logger("hello");

	lv::log::add_stdio_gather(log);
	lv::log::add_file_gather(log, "log.txt");

	lv::log::add_debug_string_gather(log)->set_filter(lv::log::DebugFilter());

	log(lv::info) << L"hello world";
	log(lv::debug) << "debug something " << 100;

	log(lv::warning) << boost::wformat(L"a number:%d") % 20;

	log() << std::endl << L"0x" << std::hex << 0x123FA << std::endl << std::flush;
}