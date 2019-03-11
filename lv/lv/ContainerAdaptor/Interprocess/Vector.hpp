// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 2012/06/01
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>

#include <boost/interprocess/containers/vector.hpp>


namespace lv::container_adaptor
{
	template<class T, class Ax>
	struct container_category<boost::interprocess::vector<T, Ax> >
		: virtual public contiguous_sequence_tag
		, virtual public back_insertion_sequence_tag
	{
	};
}
