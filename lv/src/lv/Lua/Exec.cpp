#include <lv/Lua/Exec.hpp>

#include <cstring>
#include <stdexcept>

#include <lua.hpp>


namespace lv { namespace lua {

	void dostr(lua_State * L, char const * str, pcall_handler h /* = default_pcall_handler */)
	{
		lua_pushcclosure(L, h, 0);

		if (luaL_loadbuffer(L, str, std::strlen(str), str))
		{
			std::runtime_error err(lua_tostring(L, -1));
			lua_pop(L, 2);
			throw err;
		}

		if (lua_pcall(L, 0, 0, -2))
		{
			std::runtime_error err(lua_tostring(L, -1));
			lua_pop(L, 2);
			throw err;
		}

		lua_pop(L, 1);
	}

} }