// *********************************************************************
//  String   version:  1.0   ��  date: 2012/05/20
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_INTERPROCESS_STRING_HPP
#define LV_BINARYSTREAM_INTERPROCESS_STRING_HPP

#include <boost/interprocess/containers/string.hpp>

#include <lv/BinaryStream/Serializer.hpp>

namespace lv { namespace bstream {

	template<class C, class Tr, class Ax>
	struct object_tag<boost::interprocess::basic_string<C, Tr, Ax> > : primitive_buffer_tag {};

} }

#endif