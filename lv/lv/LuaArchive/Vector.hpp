// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_VECTOR_HPP
#define LV_LUAARCHIVE_VECTOR_HPP

#include <vector>

#include <lv/LuaArchive/Tags.hpp>
#include <lv/ContainerAdaptor/Vector.hpp>

namespace lv { namespace lua { namespace archive {


	template<typename T, typename Ax>
	struct object_tag<std::vector<T, Ax> >
		: sequence_tag
	{
	};


} } }



#endif