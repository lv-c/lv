// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_VECTOR_HPP
#define LV_BINARYSTREAM_VECTOR_HPP


#include <lv/BinaryStream/Tags.hpp>

#include <vector>


namespace lv::bstream
{
	template<class T, class Ax>
	struct object_tag<std::vector<T, Ax>, std::enable_if_t<is_primitive_v<T> > >
		: primitive_buffer_tag
	{
	};

	template<class T, class Ax>
	struct object_tag<std::vector<T, Ax>, std::enable_if_t<!is_primitive_v<T> > >
		: range_tag
	{
	};

}

#endif