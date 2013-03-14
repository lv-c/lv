// *********************************************************************
//  List   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_LIST_HPP
#define LV_BINARYSTREAM_LIST_HPP


#include <list>

#include <lv/BinaryStream/Serializer.hpp>

namespace lv { namespace bstream {

	template<typename T, typename Ax>
	struct object_tag<std::list<T, Ax> >
		: range_tag
	{
	};

} }

#endif