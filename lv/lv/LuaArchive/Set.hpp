// *********************************************************************
//  Set   version:  1.0   ¡¤  date: 10/27/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_SET_HPP
#define LV_LUAARCHIVE_SET_HPP

#include <set>

#include <lv/LuaArchive/Tags.hpp>

namespace lv { namespace lua { namespace archive {

	template<typename T, typename Pr, typename Ax>
	struct object_tag<std::set<T, Pr, Ax> >
		: sequence_tag
	{
	};

} } }

#endif