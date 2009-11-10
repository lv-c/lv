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

#include <lv/BinaryStream/Serialize.hpp>

namespace lv { namespace bstream {

	template<typename T>
	struct object_tag<std::vector<T>, typename boost::enable_if<is_copyable<T> >::type>
		: copyable_buffer_tag
	{
	};

	template<typename T>
	struct object_tag<std::vector<T>, typename boost::disable_if<is_copyable<T> >::type>
		: range_tag
	{
	};

} }

#endif // LV_BINARYSTREAM_VECTOR_HPP