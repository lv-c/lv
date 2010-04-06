// *********************************************************************
//  String   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_STRING_HPP
#define LV_BINARYSTREAM_STRING_HPP

#include <string>
#include <lv/BinaryStream/Serializer.hpp>

namespace lv { namespace bstream {

	template<>
	struct object_tag<std::string> : primitive_buffer_tag {};

	template<>
	struct object_tag<std::wstring> : primitive_buffer_tag {};

} }

#endif // LV_BINARYSTREAM_STRING_HPP