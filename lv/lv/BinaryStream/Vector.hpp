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


#include <vector>

#include <lv/BinaryStream/Serializer.hpp>

namespace lv { namespace bstream {

	template<typename T, typename Ax>
	struct object_tag<std::vector<T, Ax>, typename boost::enable_if<is_primitive<T> >::type>
		: primitive_buffer_tag
	{
	};

	template<typename T, typename Ax>
	struct object_tag<std::vector<T, Ax>, typename boost::disable_if<is_primitive<T> >::type>
		: range_tag
	{
	};

} }

#endif