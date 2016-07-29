// *********************************************************************
//  Ensure   version:  1.0   ¡¤  date: 2013/06/27
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ENSURE_HPP
#define LV_ENSURE_HPP

// TODO : make it better. LV_ENSURE(check, msg) (v0) (v1) ?

#include <string>
#include <stdexcept>
#include <type_traits>


namespace lv { namespace detail {

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
	template<typename T>
	typename std::enable_if<std::is_base_of<std::exception, T>::value>::type	ensure_throw(T const & ex)
	{
		throw ex;
	}
	
} }

#define LV_ENSURE(check, msg)	\
	if (! (check)) lv::detail::ensure_throw(msg);


#endif