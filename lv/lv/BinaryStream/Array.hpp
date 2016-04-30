// *********************************************************************
//  Array   version:  1.0   ¡¤  date: 06/13/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_ARRAY_HPP
#define LV_BINARYSTREAM_ARRAY_HPP

#include <lv/BinaryStream/Serializer.hpp>

#include <array>

namespace lv { namespace bstream {

	template<typename T, size_t N>
	struct object_tag<std::array<T, N>, typename boost::enable_if<is_primitive<T> >::type>
		: primitive_buffer_tag
	{
	};

	template<typename T, size_t N>
	struct object_tag<std::array<T, N>, typename boost::disable_if<is_primitive<T> >::type>
		: range_tag
	{
	};

} }


#endif