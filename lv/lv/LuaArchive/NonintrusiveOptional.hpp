// *********************************************************************
//  NonintrusiveOptional   version:  1.0   ��  date: 2014/03/09
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_NONINTRUSIVE_OPTIONAL_HPP
#define LV_LUAARCHIVE_NONINTRUSIVE_OPTIONAL_HPP

#include <lv/Serialization/NonintrusiveOptional.hpp>

namespace lv { namespace lua { namespace archive {


	template<typename T>
	void	load(luabind::object const & obj, lv::serialization::NonintrusiveOptional<T> & t)
	{
		if(luabind::type(obj) != LUA_TNIL)
		{
			load(obj, t.value());
		}
	}

} } }

#endif