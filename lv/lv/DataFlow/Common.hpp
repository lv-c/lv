// *********************************************************************
//  Common   version:  1.0   ¡¤  date: 10/24/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_COMMON_HPP
#define LV_DATAFLOW_COMMON_HPP

#include <boost/function.hpp>

#include <lv/Buffer.hpp>

namespace lv { namespace flow {

	typedef boost::function<void(BufferPtr)>	Slot;

} }

#endif