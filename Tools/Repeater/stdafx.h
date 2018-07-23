#pragma once

#define _SCL_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING

#pragma warning(disable : 4819)

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


#ifdef _DEBUG
#pragma comment(lib, "libluabindd.lib")
#pragma comment(lib, "libluad.lib")
#else
#pragma comment(lib, "libluabind.lib")
#pragma comment(lib, "liblua.lib")
#endif
