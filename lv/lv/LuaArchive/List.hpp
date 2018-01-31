// *********************************************************************
//  List   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <list>

#include <lv/LuaArchive/Tags.hpp>
#include <lv/ContainerAdaptor/List.hpp>


namespace lv::lua::archive
{
	template<class T, class Ax>
	struct object_tag<std::list<T, Ax> >
		: sequence_tag
	{
	};


}
