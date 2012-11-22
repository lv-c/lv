// *********************************************************************
//  Deque   version:  1.0   ¡¤  date: 2012/11/22
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_DEQUE_HPP
#define LV_LUAARCHIVE_DEQUE_HPP

#include <deque>

#include <lv/LuaArchive/Tags.hpp>
#include <lv/ContainerAdaptor/Deque.hpp>

namespace lv { namespace lua { namespace archive {


	template<typename T, typename Ax>
	struct object_tag<std::deque<T, Ax> >
		: sequence_tag
	{
	};


} } }


#endif