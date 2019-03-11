// *********************************************************************
//  List   version:  1.0   ¡¤  date: 2012/05/10
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>

#include <list>


namespace lv::container_adaptor
{
	template<class T, class Ax>
	struct container_category<std::list<T, Ax> >
		: virtual public back_insertion_sequence_tag
	{
	};
}
