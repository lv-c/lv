// *********************************************************************
//  SharedPtr   version:  1.0   ¡¤  date: 11/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

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
