// *********************************************************************
//  Point   version:  1.0   ¡¤  date: 11/25/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Graphics/Point.hpp>


namespace boost::serialization
{
	template<class Archive, class T>
	void serialize(Archive & ar, lv::PointT<T> & pt, unsigned int)
	{
		ar & pt.x & pt.y;
	}

}