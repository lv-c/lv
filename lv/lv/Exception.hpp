// *********************************************************************
//  Exception   version:  1.0   ¡¤  date: 12/03/2007
//  --------------------------------------------------------------------
//		 Helpers for defining exceptions
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_EXCEPTION_HPP
#define LV_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS
#include <Windows.h>
#include <sstream>
#include <dxerr9.h>

#endif	// LV_PLATFORM_WINDOWS

namespace lv
{

#define DEFINE_EXCEPTION(excp, base, default_msg)		\
	class excp : public base							\
	{													\
	public:												\
		explicit excp(std::string const & msg) throw()	\
			: base(msg)									\
		{												\
		}												\
														\
		excp()	throw()									\
			: base(default_msg)							\
		{												\
		}												\
														\
	}													

	// use the class name as the default message
#define DEFINE_EXCEPTION_MSG(excp, base)					\
	DEFINE_EXCEPTION(excp, base, #excp)			// # (stringize) and ## (token catenation)


	// Defines some common exceptions
	DEFINE_EXCEPTION_MSG(io_error, std::runtime_error);
	DEFINE_EXCEPTION_MSG(file_io_error, io_error);


#ifdef LV_PLATFORM_WINDOWS

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

#endif	// LV_PLATFORM_WINDOWS
}


#endif // LV_EXCEPTION_HPP