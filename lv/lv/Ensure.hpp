// *********************************************************************
//  Ensure   version:  1.0   ¡¤  date: 2013/06/27
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

// TODO : make it better. LV_ENSURE(check, msg) (v0) (v1) ?

#include <string>
#include <stdexcept>
#include <type_traits>

#include <boost/assert.hpp>


namespace lv::detail
{
	[[noreturn]] inline void	ensure_throw(char const * msg)
	{
		throw std::runtime_error(msg);
	}

	[[noreturn]] inline void	ensure_throw(std::string const & msg)
	{
		throw std::runtime_error(msg);
	}

	// we can't do it like this: void ensure_throw(std::exception const & ex) { throw ex; }
	// ex will be copied so we must known the real type
	template<class T>
	[[noreturn]] std::enable_if_t<std::is_base_of_v<std::exception, T> >	ensure_throw(T const & ex)
	{
		throw ex;
	}
	
}

#define LV_ENSURE(check, msg)			\
	if (!(check)) {					\
		BOOST_ASSERT(false);			\
		lv::detail::ensure_throw(msg);	\
	}