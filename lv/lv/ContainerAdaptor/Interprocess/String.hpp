// *********************************************************************
//  String   version:  1.0   ¡¤  date: 2019/01/09
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>

#include <boost/interprocess/containers/string.hpp>


namespace lv::container_adaptor
{
	template<class C, class Tr, class Ax>
	struct container_category<boost::interprocess::basic_string<C, Tr, Ax> >
		: virtual public contiguous_sequence_tag
		, virtual public back_insertion_sequence_tag
	{
	};
}

