#include <lv/Toolkit/Lua/LuaConfig.hpp>
#include <lv/FileSystem/RawFileReader.hpp>
#include <lv/Lua/Exec.hpp>
#include <lv/SharedPtr.hpp>
#include <lv/LuaArchive/LuaIArchive.hpp>

#include <lua.hpp>
#include <luabind/luabind.hpp>

namespace lv { namespace lua {

	LuaConfig::LuaConfig(std::string const & file /* = std::string() */, IFileIOPtr file_loader /* = IFileIOPtr */)
		: file_loader_(file_loader)
	{
		if(! this->file_loader_)
		{
			this->file_loader_.reset(new RawFileReader());
		}

		L_ = init_lua();

		ia_.reset(new LuaIArchive(luabind::globals(L_)));

		//
		if(! file.empty())
		{
			load_file(file);
		}
	}

	LuaConfig::~LuaConfig()
	{
		ia_.reset();
		lua_close(L_);
	}

	lua_State * LuaConfig::init_lua()
	{
		lua_State * state = luaL_newstate();

		luaopen_base(state);
		luaL_openlibs(state);

		luabind::open(state);

		return state;
	}

	LuaIArchive const & LuaConfig::archive() const
	{
		return *ia_;
	}

	lua_State * LuaConfig::lua_state()
	{
		return L_;
	}

	void LuaConfig::load_file(std::string const & file)
	{
		Buffer buf;
		file_loader_->fulfill(file, lv::shared_from_object(buf));

		load_str(buffer::data(buf), buf.size());
	}

	void LuaConfig::load_str(char const * str, size_t size)
	{
		lua::dostr(L_, str, size);
	}

} }