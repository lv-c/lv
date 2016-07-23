// *********************************************************************
//  SharedPtr   version:  1.0   ¡¤  date: 11/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SHAREDPTR_HPP
#define LV_SHAREDPTR_HPP

#include <memory>


namespace lv
{
	/// shared_from_object
	template<class T>
	std::shared_ptr<T> shared_from_object(T & t)
	{
		// null deleter
		return std::shared_ptr<T>(&t, [](void const *) {});
	}
}

#endif
