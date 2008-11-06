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

namespace lv
{
	typedef boost::shared_ptr<std::istream>	IStreamPtr;

	typedef boost::shared_ptr<std::ostream>	OStreamPtr;
}

#endif // LV_STREAMPTR_HPP