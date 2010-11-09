// *********************************************************************
//  Map   version:  1.0   ��  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_MAP_HPP
#define LV_LUAARCHIVE_MAP_HPP

#include <map>

#include <lv/LuaArchive/Tags.hpp>
#include <lv/LuaArchive/Pair.hpp>

namespace lv { namespace lua { namespace archive {

	template<typename K, typename V, typename Pr, typename Ax>
	struct object_tag<std::map<K, V, Pr, Ax> >
		: unordered_tag
	{
	};


} } }



#endif