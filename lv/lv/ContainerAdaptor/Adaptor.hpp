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
	void	insert(T & t, V && v)
	{
		t.insert(std::forward<V>(v));
	}

	template<class T, class V>
	void	push_back(T & t, V && v)
	{
		t.push_back(std::forward<V>(v));
	}

	template<class T, class V>
	void	push_front(T & t, V && v)
	{
		t.push_front(std::forward<V>(v));
	}

	template<class T, class I>
	void	assign(T & t, I const & begin, I const & end)
	{
		t.assign(begin, end);
	}

	template<class T>
	void	resize(T & t, size_t size)
	{
		t.resize(size);
	}

	template<class T>
	void	reserve(T & t, size_t size)
	{
		t.reserve(size);
	}
}

#endif