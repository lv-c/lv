// *********************************************************************
//  Set   version:  1.0   ¡¤  date: 10/27/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <set>

#include <lv/LuaArchive/Tags.hpp>


namespace lv::lua::archive
{
	template<class T, class Pr, class Ax>
	struct object_tag<std::set<T, Pr, Ax> >
		: sequence_tag
	{
	};

}