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


namespace lv
{

#define DEFINE_EXCEPTION(excp, base, default_msg)		\
	class excp : public base							\
	{													\
	public:												\
		explicit excp(std::string const & msg)			\
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

}


#endif // LV_EXCEPTION_HPP