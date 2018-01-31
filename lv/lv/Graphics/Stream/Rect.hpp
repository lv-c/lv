// *********************************************************************
//  Rect   version:  1.0   ¡¤  date: 09/07/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Rect.hpp>

namespace lv
{
	template<class T>
	std::ostream & operator << (std::ostream & stream, RectT<T> const & rc)
	{
		return stream << "Rect(" << rc.left << ", " << rc.top << ", " << rc.right
			<< ", " << rc.bottom << ')';
	}
}