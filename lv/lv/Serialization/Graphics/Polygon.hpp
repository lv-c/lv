// *********************************************************************
//  Polygon   version:  1.0   ¡¤  date: 2011/05/11
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2011 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_GRAPHICS_POLYGON_HPP
#define LV_SERIALIZATION_GRAPHICS_POLYGON_HPP

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_free.hpp>

#include <boost/polygon/point_data.hpp>

namespace boost { namespace serialization {

	template<typename Archive, typename T>
	void save(Archive & ar, boost::polygon::point_data<T> const & pt, unsigned int)
	{
		ar & pt.x() & pt.y();
	}

	template<typename Archive, typename T>
	void load(Archive & ar, boost::polygon::point_data<T> & pt, unsigned int)
	{
		T x, y;
		ar & x & y;

		pt.x(x);
		pt.y(y);
	}

	template<typename Archive, typename T>
	void serialize(Archive & ar, boost::polygon::point_data<T> & pt, unsigned int version)
	{
		split_free(ar, pt, version);
	}

} }

#endif