// *********************************************************************
//  Map   version:  1.0   ¡¤  date: 09/19/2010
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


namespace lv::lua::archive
{
	template<class K, class V, class Pr, class Ax>
	struct object_tag<std::map<K, V, Pr, Ax> >
		: sequence_tag
	{
	};


}


#endif