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

#include <lv/BinaryStream/Tags.hpp>

#include <string>


namespace lv::bstream
{
	template<class C, class Tr, class Ax>
	struct object_tag<std::basic_string<C, Tr, Ax> > : primitive_buffer_tag {};

}

#endif