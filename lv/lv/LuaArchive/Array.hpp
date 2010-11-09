// *********************************************************************
//  Array   version:  1.0   ��  date: 11/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_ARRAY_HPP
#define LV_LUAARCHIVE_ARRAY_HPP

#include <lv/Exception.hpp>

#include <boost/array.hpp>

namespace lv { namespace lua { namespace archive {

	DEFINE_EXCEPTION_MSG(ArraySizeTooShort, std::out_of_range)

	template<typename T, size_t N>
	void	save(std::ostream & os, boost::array<T, N> const & v, size_t level)
	{
		detail::save_range(os, v.begin(), v.end(), level);
	}

	/// @exception ArraySizeTooShort
	template<typename T, size_t N>
	void	load(luabind::object const & obj, boost::array<T, N> & v)
	{
		expect_obj_type(obj, LUA_TTABLE);

		size_t index(0);
		for(luabind::iterator it(obj), end; it != end; ++it, ++index)
		{
			if(index >= N)
				throw ArraySizeTooShort();

			BOOST_ASSERT(! detail::is_version_key(it.key()) && "you shouldn't place a version key here");

			load_item(it, v[index]);
		}
	}


} } }

#endif