#include <lv/Lua/Exec.hpp>
#include <lv/Lua/StackBalance.hpp>

#include <cstring>
#include <stdexcept>

#include <lua.hpp>


namespace lv { namespace lua {

	void dostr(lua_State * L, char const * str, size_t size, char const * name /* = nullptr */, pcall_handler h /* = default_pcall_handler */)
	{
		if (name == nullptr)
		{
			name = str;
		}

		StackBalance balance(L);

		lua_pushcclosure(L, h, 0);

		if (luaL_loadbuffer(L, str, size, name))
		{
			std::runtime_error err(lua_tostring(L, -1));
			throw err;
		}

		if (lua_pcall(L, 0, 0, -2))
		{
			std::runtime_error err(lua_tostring(L, -1));
			throw err;
		}
	}

} }