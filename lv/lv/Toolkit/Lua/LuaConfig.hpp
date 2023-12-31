// *********************************************************************
//  LuaConfig   version:  1.0   ��  date: 10/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Toolkit/Lua/Fwd.hpp>
#include <lv/Toolkit/AutoLink.hpp>
#include <lv/LuaArchive/LuaIArchive.hpp>
#include <lv/FileSystem/Fwd.hpp>

#include <string>


struct lua_State;

namespace lv::lua
{
	/// you can call luabind::set_pcall_callback to catch the errors

	class LuaConfig
	{
		lua_State *	L_;

		IFileIOPtr	file_loader_;

		std::unique_ptr<LuaIArchive>	ia_;

	public:

		/// If you don't specify a file loader, RawFileReader will be used
		explicit LuaConfig(std::string const & file = std::string(), IFileIOPtr file_loader = IFileIOPtr());

		virtual ~LuaConfig();

		/// @exception runtime_error
		void	load_file(std::string const & file);

		/// @exception runtime_error
		void	load_str(char const * str, size_t size);

		LuaIArchive	const &	archive() const;

		lua_State *	lua_state();

		template<class T>
		LuaConfig const & operator () (char const * name, T & t) const
		{
			*ia_ & boost::serialization::make_nvp(name, t);
			return *this;
		}

		template<class T>
		LuaConfig const & operator () (T & t) const
		{
			archive::load(luabind::globals(L_), t);
			return *this;
		}

	private:

		lua_State *	init_lua();

	};

}