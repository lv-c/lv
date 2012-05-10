// *********************************************************************
//  List   version:  1.0   ¡¤  date: 2012/05/10
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONTAINERADAPTOR_LIST_HPP
#define LV_CONTAINERADAPTOR_LIST_HPP

#include <lv/ContainerAdaptor/Adaptor.hpp>

#include <list>

namespace lv
{
	template<class T, class V>
	void	insert(std::list<T> & t, V const & v)
	{
		t.push_back(v);
	}
}

#endif