// *********************************************************************
//  Size   version:  1.0   ��  date: 09/07/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Size.hpp>

namespace lv
{
	template<class T>
	std::ostream & operator << (std::ostream & stream, SizeT<T> const & sz)
	{
		return stream << "Size(" << sz.cx << ", " << sz.cy << ')';
	}
}