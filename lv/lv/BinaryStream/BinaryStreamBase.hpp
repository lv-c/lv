// *********************************************************************
//  BinaryStreamBase   version:  1.0   ¡¤  date: 2012/03/09
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/noncopyable.hpp>


namespace lv::bstream
{
	class BinaryStreamBase : boost::noncopyable
	{
		bool	switch_endian_ = false;

	public:

		bool	switch_endian() const
		{
			return switch_endian_;
		}

		void	switch_endian(bool s)
		{
			switch_endian_ = s;
		}
	};
}
