// *********************************************************************
//  Rect   version:  1.0   ¡¤  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Rect.hpp>


namespace boost::serialization
{
	template<class Archive, class T>
	void serialize(Archive & ar, lv::RectT<T> & rc, unsigned int)
	{
		ar & rc.left & rc.top & rc.right & rc.bottom;
	}

}