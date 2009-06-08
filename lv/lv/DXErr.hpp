// *********************************************************************
//  DXErr   version:  1.0   ¡¤  date: 05/15/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DXERR_HPP
#define LV_DXERR_HPP

#include <lv/Config.hpp>


#ifdef LV_PLATFORM_WINDOWS

#include <stdexcept>
#include <sstream>

#include <Windows.h>
#include <dxerr9.h>

#include <boost/assert.hpp>


namespace detail
{
	inline std::string file_line_hr(std::string const & file, int line, HRESULT hr)
	{
		std::ostringstream 	oss;
		oss << file << " : " << line << " : " << DXGetErrorString9A(hr);
		return oss.str();
	}
}

// DirectX. throw if failed
#define DX_TIF(expr) { HRESULT _hr = expr; if(FAILED(_hr)) { throw std::runtime_error(detail::file_line_hr(__FILE__, __LINE__, _hr));} }

#define DX_VERIFY(expr) BOOST_VERIFY(SUCCEEDED(expr))

#endif	// LV_PLATFORM_WINDOWS

#endif // LV_DXERR_HPP