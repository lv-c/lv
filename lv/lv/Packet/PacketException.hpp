// *********************************************************************
//  PacketException   version:  1.0   ¡¤  date: 09/13/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_PACKETEXCEPTION_HPP
#define LV_PACKETEXCEPTION_HPP


#include <boost/archive/archive_exception.hpp>

namespace lv
{

	class PacketException : public boost::archive::archive_exception
	{
	public:

		PacketException(exception_code c)
			: boost::archive::archive_exception(c)
		{
		}

		PacketException()
		{
		}

	};

}

#endif // LV_PACKETEXCEPTION_HPP