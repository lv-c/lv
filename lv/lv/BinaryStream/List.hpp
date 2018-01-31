// *********************************************************************
//  List   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/BinaryStream/Tags.hpp>

#include <list>


namespace lv::bstream
{
	template<class T, class Ax>
	struct object_tag<std::list<T, Ax> >
		: range_tag
	{
	};

}