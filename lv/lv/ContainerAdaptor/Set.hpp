// *********************************************************************
//  Set   version:  1.0   ¡¤  date: 2019/01/08
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>

#include <set>


namespace lv::container_adaptor
{
	template<class T, class Pr, class Ax>
	struct container_category<std::set<T, Pr, Ax> >
		: virtual public associate_tag
	{
	};
}