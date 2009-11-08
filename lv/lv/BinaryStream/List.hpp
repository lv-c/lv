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

#include <lv/BinaryStream/Serialize.hpp>

namespace lv { namespace bstream {

	template<typename>
	struct object_tag<std::list<T> >
		: range_tag
	{
	};

} }

#endif // LV_BINARYSTREAM_LIST_HPP