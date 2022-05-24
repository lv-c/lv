#ifndef REPEATER_STDAFX_H
#define REPEATER_STDAFX_H

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT	0x0501


#include <lv/Config.hpp>
#include <lv/Log/LogManager.hpp>

using namespace lv;

#define LOG LogManager::instance().logger()


#include <array>

using namespace std;

#include <boost/lexical_cast.hpp>
#include <boost/asio/deadline_timer.hpp>


#define LV_LIB_NAME "luabind"
#include <lv/AutoLink.hpp>

#define LV_LIB_NAME "lua"
#include <lv/AutoLink.hpp>

#endif
