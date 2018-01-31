// *********************************************************************
//  Map   version:  1.0   ¡¤  date: 2012/06/01
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/interprocess/containers/map.hpp>

#include <lv/LuaArchive/Tags.hpp>
#include <lv/LuaArchive/Pair.hpp>


namespace lv::lua::archive
{
	template<class K, class V, class Pr, class Ax>
	struct object_tag<boost::interprocess::map<K, V, Pr, Ax> >
		: sequence_tag
	{
	};

}
