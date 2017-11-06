#include <lv/Lua/StackBalance.hpp>

#include <lua.hpp>

namespace lv::lua
{
	StackBalance::StackBalance(lua_State * L)
		: top_(lua_gettop(L))
		, L_(L)
	{
	}

	StackBalance::~StackBalance()
	{
		lua_settop(L_, top_);
	}

}