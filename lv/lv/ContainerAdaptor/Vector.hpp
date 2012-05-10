// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 2012/05/10
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONTAINERADAPTOR_VECTOR_HPP
#define LV_CONTAINERADAPTOR_VECTOR_HPP

#include <lv/ContainerAdaptor/Adaptor.hpp>

#include <vector>

namespace lv
{
	template<class T, class V>
	void	insert(std::vector<T> & t, V const & v)
	{
		t.push_back(v);
	}

	template<class T>
	void	reserve(std::vector<T> & t, size_t size)
	{
		t.reserve(size);
	}
}

#endif