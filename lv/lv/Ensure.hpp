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

// TODO : make it better

#include <string>
#include <stdexcept>

namespace lv { namespace detail {

	inline void	ensure_throw(char const * msg)
	{
		throw std::runtime_error(msg);
	}

	inline void	ensure_throw(std::string const & msg)
	{
		throw std::runtime_error(msg);
	}

	inline void	ensure_throw(std::exception const & ex)
	{
		throw ex;
	}
	
} }

#define LV_ENSURE(check, msg)	\
	if(! (check)) lv::detail::ensure_throw(msg);


#endif