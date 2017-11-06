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

#include <lv/BinaryStream/Tags.hpp>

#include <array>


namespace lv::bstream
{
	template<typename T, size_t N>
	struct object_tag<std::array<T, N>, std::enable_if_t<is_primitive<T>::value> >
		: primitive_buffer_tag
	{
	};

	template<typename T, size_t N>
	struct object_tag<std::array<T, N>, std::enable_if_t<! is_primitive<T>::value> >
		: range_tag
	{
	};

}


#endif