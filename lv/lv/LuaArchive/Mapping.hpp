// *********************************************************************
//  Mapping   version:  1.0   ¡¤  date: 2013/02/25
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_MAPPING_HPP
#define LV_LUAARCHIVE_MAPPING_HPP

#include <lv/Mapping.hpp>
#include <lv/LuaArchive/Tags.hpp>
#include <lv/LuaArchive/Pair.hpp>

namespace lv::lua::archive
{
	template<typename X, typename Y>
	struct object_tag<Mapping<X, Y> >
		: sequence_tag
	{
	};


}

#endif