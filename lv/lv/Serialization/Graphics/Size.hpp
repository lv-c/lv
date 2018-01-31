// *********************************************************************
//  Size   version:  1.0   ¡¤  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Size.hpp>


namespace boost::serialization
{
	template<class Archive, class T>
	void serialize(Archive & ar, lv::SizeT<T> & sz, unsigned int)
	{
		ar & sz.cx & sz.cy;
	}

}