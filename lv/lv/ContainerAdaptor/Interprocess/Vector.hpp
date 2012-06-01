// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 2012/06/01
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONTAINERADAPTOR_INTERPROCESS_VECTOR_HPP
#define LV_CONTAINERADAPTOR_INTERPROCESS_VECTOR_HPP

#include <lv/ContainerAdaptor/Adaptor.hpp>

#include <boost/interprocess/containers/vector.hpp>

namespace lv
{
	template<class T, class Ax, class V>
	void	insert(boost::interprocess::vector<T, Ax> & t, V const & v)
	{
		t.push_back(v);
	}

	template<class T>
	void	reserve(boost::interprocess::vector<T, Ax> & t, size_t size)
	{
		t.reserve(size);
	}
}

#endif