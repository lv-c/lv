// *********************************************************************
//  Array   version:  1.0   ¡¤  date: 11/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_ARRAY_HPP
#define LV_LUAARCHIVE_ARRAY_HPP

#include <lv/LuaArchive/Fwd.hpp>
#include <lv/Exception.hpp>
#include <lv/Ensure.hpp>

#include <array>

namespace lv { namespace lua { namespace archive {

	DEFINE_EXCEPTION_MSG(ArraySizeTooShort, std::out_of_range)

	template<typename T, size_t N>
	void	save(std::ostream & os, std::array<T, N> const & v, size_t level)
	{
		detail::save_range(os, v.begin(), v.end(), level);
	}

	/// @exception ArraySizeTooShort
	template<typename T, size_t N>
	void	load(luabind::object const & obj, std::array<T, N> & v)
	{
		expect_obj_type(obj, LUA_TTABLE);

		size_t index(0);
		for (luabind::iterator it(obj), end; it != end; ++it, ++index)
		{
			LV_ENSURE(index < N, ArraySizeTooShort());

			BOOST_ASSERT(! detail::is_version_key(it.key()) && "you shouldn't place a version key here");

			load_item(it, v[index]);
		}
	}

	template<typename T, size_t N>
	void	load(Parser & parser, std::array<T, N> & v)
	{
		parser >> detail::symbol('{');

		int index = 0;

		while (! parser.read_if('}'))
		{
			LV_ENSURE(index < N, ArraySizeTooShort());
			parser >> v[index++];
		}
	}


} } }

#endif