// *********************************************************************
//  String   version:  1.0   ¡¤  date: 2012/05/20
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/BinaryStream/Tags.hpp>

#include <boost/interprocess/containers/string.hpp>


namespace lv::bstream
{
	template<class C, class Tr, class Ax>
	struct object_tag<boost::interprocess::basic_string<C, Tr, Ax> > : primitive_buffer_tag {};

}