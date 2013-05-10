#pragma once

#define _SCL_SECURE_NO_WARNINGS

#pragma warning(disable : 4819)

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT	0x0501


#include <lv/Log/LogManager.hpp>

using namespace lv;

#define LOG LogManager::instance().logger()


using namespace std;


#ifdef _DEBUG
#pragma comment(lib, "libluabindd.lib")
#else
#pragma comment(lib, "libluabind.lib")
#endif
#pragma comment(lib, "liblua.lib")
