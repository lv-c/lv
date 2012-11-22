// *********************************************************************
//  Deque   version:  1.0   ¡¤  date: 2012/11/22
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONTAINERADAPTOR_DEQUE_HPP
#define LV_CONTAINERADAPTOR_DEQUE_HPP

#include <lv/ContainerAdaptor/Adaptor.hpp>

#include <deque>

namespace lv
{
	template<class T, class Ax, class V>
	void	insert(std::deque<T, Ax> & t, V const & v)
	{
		t.push_back(v);
	}
}

#endif