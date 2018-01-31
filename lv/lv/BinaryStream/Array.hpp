// *********************************************************************
//  Array   version:  1.0   ¡¤  date: 06/13/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/BinaryStream/Tags.hpp>

#include <array>


namespace lv::bstream
{
	template<class T, size_t N>
	struct object_tag<std::array<T, N>, std::enable_if_t<is_primitive_v<T> > >
		: primitive_buffer_tag
	{
	};

	template<class T, size_t N>
	struct object_tag<std::array<T, N>, std::enable_if_t<!is_primitive_v<T> > >
		: range_tag
	{
	};

}
