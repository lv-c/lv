// *********************************************************************
//  SingletonExtractor   version:  1.0   ¡¤  date: 12/04/2008
//  --------------------------------------------------------------------
//  Parameter extractor for singleton (using lv::Singleton) objects
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SINGLETONEXTRACTOR_HPP
#define LV_SINGLETONEXTRACTOR_HPP

#include <lv/rpc/Ref.hpp>

namespace lv { namespace rpc {


	template<class T>
	struct SingletonExtractor
	{
		// we can't use boost::reference_wrapper here because it don't have 
		// a default constructor
		typedef RefWrapper<T>	type;

		template<class IArchive>
		void operator () (IArchive &, type & val) const
		{
			val = boost::ref(T::instance());
		}
	};

} }

#endif // LV_SINGLETONEXTRACTOR_HPP