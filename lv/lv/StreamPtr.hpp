// *********************************************************************
//  StreamPtr   version:  1.0   ¡¤  date: 10/19/2008
//  --------------------------------------------------------------------
//
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAMPTR_HPP
#define LV_STREAMPTR_HPP


#include <iostream>

#include <boost/shared_ptr.hpp>

#ifdef BOOST_SP_USE_QUICK_ALLOCATOR
#include <lv/SharedPtr.hpp>
#endif


namespace lv
{
	typedef boost::shared_ptr<std::istream>	IStreamPtr;

	typedef boost::shared_ptr<std::ostream>	OStreamPtr;

#ifdef BOOST_SP_USE_QUICK_ALLOCATOR

	namespace detail
	{
		static boost::shared_ptr<std::istream> dummy_istream((std::istream*)0);
		static boost::shared_ptr<std::istream> dummy_istream_null((std::istream*)0, NullDeleter());

		static boost::shared_ptr<std::ostream> dummy_ostream((std::ostream*)0);
		static boost::shared_ptr<std::ostream> dummy_ostream_null((std::ostream*)0, NullDeleter());
	}

#endif

}

#endif