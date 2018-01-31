// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 2012/05/10
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Adaptor.hpp>

#include <vector>

namespace lv
{
	template<class T, class Ax, class V>
	void	insert(std::vector<T, Ax> & t, V && v)
	{
		t.push_back(std::forward<V>(v));
	}
}