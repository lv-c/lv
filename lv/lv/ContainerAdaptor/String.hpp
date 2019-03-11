// *********************************************************************
//  String   version:  1.0   ¡¤  date: 2019/01/08
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>

#include <string>


namespace lv::container_adaptor
{
	template<class C, class Tr, class Ax>
	struct container_category<std::basic_string<C, Tr, Ax> >
		: virtual public contiguous_sequence_tag
		, virtual public back_insertion_sequence_tag
	{
	};
}
