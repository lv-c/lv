// *********************************************************************
//  Mapping   version:  1.0   ¡¤  date: 2019/01/08
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>
#include <lv/Mapping.hpp>


namespace lv::container_adaptor
{
	template<class X, class Y>
	struct container_category<Mapping<X, Y> >
		: virtual public associate_tag
	{
	};
}