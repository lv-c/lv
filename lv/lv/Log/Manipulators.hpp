// *********************************************************************
//  Manipulators   version:  1.0   ¡¤  date: 08/06/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LOG_MANIPULATORS_HPP
#define LV_LOG_MANIPULATORS_HPP

#include <lv/Log/Fwd.hpp>

namespace lv { namespace log {

	namespace detail
	{
		template<typename T>
		class hex_impl
		{
			T	t_;
		public:
			hex_impl(T t) : t_(t) {}

			friend ostream_type & operator << (ostream_type & os, hex_impl const & h)
			{
				return os << "0x" << std::hex << h.t_ << std::dec;
			}
		};
	}

	
	template<typename T>
	detail::hex_impl<T> hex(T t)
	{
		return detail::hex_impl<T>(t);
	}

} }


#endif