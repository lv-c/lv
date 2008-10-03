// *********************************************************************
//  NullDeleter   version:  1.0   ¡¤  date: 01/03/2008
//  --------------------------------------------------------------------
//	NULL deleter¡£
//	see http://www.boost.org/libs/smart_ptr/sp_techniques.html#static
//	"Using a shared_ptr to hold a pointer to a statically allocated object"
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NULLDELETER_HPP
#define LV_NULLDELETER_HPP

#include <boost/shared_ptr.hpp>

namespace lv
{
	struct NullDeleter
	{
		void	operator() (void const *) const
		{
		}
	};

	
	template<class T>
	boost::shared_ptr<T> shared_from_object(T & t)
	{
		return boost::shared_ptr<T>(&t, NullDeleter());
	}

}

#endif // LV_NULLDELETER_HPP