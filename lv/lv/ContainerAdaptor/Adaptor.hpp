// *********************************************************************
//  Adaptor   version:  1.0   ¡¤  date: 2012/05/10
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONTAINERADAPTOR_ADAPTOR_HPP
#define LV_CONTAINERADAPTOR_ADAPTOR_HPP

namespace lv
{
	template<class T>
	void	clear(T & t)
	{
		t.clear();
	}

	template<class T, class V>
	void	insert(T & t, V const & v)
	{
		t.insert(v);
	}

	template<class T>
	void	reserve(T & t, size_t size)
	{
		t.reserve(size);
	}
}

#endif