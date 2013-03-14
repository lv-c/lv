// *********************************************************************
//  ComPtr   version:  1.0   ¡¤  date: 12/24/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_COMPTR_HPP
#define LV_COMPTR_HPP

#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS	// com_ptr

/*
If you have problem linking with this file, do as instructed (from the boost.mem_fn document):
"It is best to define these macros in the project options, via -D on the command line, 
or as the first line in the translation unit (.cpp file) where mem_fn is used. Not 
following this rule can lead to obscure errors when a header includes mem_fn.hpp before
the macro has been defined."
*/
#	ifndef BOOST_MEM_FN_ENABLE_STDCALL
#		define BOOST_MEM_FN_ENABLE_STDCALL
#	endif
#	include <boost/mem_fn.hpp>

namespace lv
{

	/// com_ptr (windows only)
	template<typename T>
	boost::shared_ptr<T>	com_ptr(T * p)
	{
		return boost::shared_ptr<T>(p, boost::mem_fn(&T::Release));
	}
}

#endif 


#endif