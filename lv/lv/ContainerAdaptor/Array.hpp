// *********************************************************************
//  Array   version:  1.0   ¡¤  date: 2019/01/10
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>

#include <array>


namespace lv::container_adaptor
{
	template<class T, size_t N>
	struct container_category<std::array<T, N> >
		: virtual public contiguous_sequence_tag
	{
	};
}