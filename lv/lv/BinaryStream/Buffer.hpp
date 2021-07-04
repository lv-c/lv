// *********************************************************************
//  Buffer   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Buffer.hpp>
#include <lv/BinaryStream/Tags.hpp>
#include <lv/BinaryStream/Vector.hpp>


namespace lv::bstream
{
	template<class T>
	struct object_tag<std::span<T>, std::enable_if_t<is_primitive_v<T> > > 
		: primitive_buffer_tag 
	{
	};

	template<class T>
	struct object_tag<std::span<T>, std::enable_if_t<!is_primitive_v<T> > >
		: range_tag
	{
	};
}