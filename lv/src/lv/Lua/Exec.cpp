#include <lv/Lua/Exec.hpp>
#include <lv/Lua/StackBalance.hpp>
#include <lv/Ensure.hpp>

#include <stdexcept>

#include <lua.hpp>


namespace lv::lua
{
	void dostr(lua_State * L, char const * str, std::size_t size, char const * name /* = nullptr */, pcall_handler h /* = default_pcall_handler */)
	{
		if (name == nullptr)
		{
			name = str;
		}

		StackBalance balance(L);

		lua_pushcclosure(L, h, 0);

		LV_ENSURE(luaL_loadbuffer(L, str, size, name) == 0, lua_tostring(L, -1));
		LV_ENSURE(lua_pcall(L, 0, 0, -2) == 0, lua_tostring(L, -1));
	}

}
