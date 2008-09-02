// *********************************************************************
//  Foreach   version:  1.0   ·  date: 07/06/2008
//  --------------------------------------------------------------------
//		定义宏 foreach 来代替 BOOST_FOREACH
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FOREACH_HPP
#define LV_FOREACH_HPP

#include <boost/foreach.hpp>

#ifndef foreach
#define foreach BOOST_FOREACH
#endif

#endif // LV_FOREACH_HPP