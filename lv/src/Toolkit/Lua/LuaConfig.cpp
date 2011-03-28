#include <lv/Toolkit/Lua/LuaConfig.hpp>
#include <lv/FileSystem/RawFileReader.hpp>
#include <lv/Lua/Exec.hpp>
#include <lv/SharedPtr.hpp>
#include <lv/LuaArchive/LuaIArchive.hpp>

#include <lua.hpp>
#include <luabind/luabind.hpp>

namespace lv { namespace lua {

	LuaConfig::LuaConfig(IFileIOPtr file_loader /* = IFileIOPtr */)
		: file_loader_(file_loader)
	{
		if(! this->file_loader_)
			this->file_loader_.reset(new RawFileReader());

		init_lua();

		ia_.reset(new LuaIArchive(luabind::globals(state_)));
	}

	LuaConfig::~LuaConfig()
	{
		ia_.reset();
		lua_close(state_);
	}

	void LuaConfig::init_lua()
	{
		state_ = lua_open();
		luaopen_base(state_);
		luaL_openlibs(state_);

		luabind::open(state_);
	}

	LuaIArchive const & LuaConfig::archive() const
	{
		return *ia_;
	}

	void LuaConfig::load_file(std::string const & file)
	{
		Buffer buf;
		file_loader_->fulfill(file, lv::shared_from_object(buf));

		BOOST_ASSERT(buf.capacity() >= buf.size() + 1);
		buf.push_back('\0');

		load_str(buffer::data(buf));
	}

	void LuaConfig::load_str(char const * str)
	{
		lua::dostr(state_, str);
	}

} }