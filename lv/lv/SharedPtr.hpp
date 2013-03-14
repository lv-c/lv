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

#include <lv/Config.hpp>

#include <boost/shared_ptr.hpp>

namespace lv
{
	/// NullDeleter
	struct NullDeleter
	{
		void	operator() (void const *) const 
		{
		}
	};

	/// shared_from_object
	template<class T>
	boost::shared_ptr<T> shared_from_object(T & t)
	{
		return boost::shared_ptr<T>(&t, NullDeleter());
	}
}

#endif
