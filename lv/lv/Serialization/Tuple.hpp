// *********************************************************************
//  Tuple   version:  1.0   ��  date: 12/15/2008
//  --------------------------------------------------------------------
//	Serializing std::tuple using boost.serialization
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <tuple>

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/std_tuple.hpp>


namespace boost::serialization
{
	template<class Archive, class ...Types>
	void serialize(Archive & ar, std::tuple<Types...> & t, unsigned int)
	{
		boost::fusion::for_each(t, [&ar](auto & v) { ar & boost::serialization::make_nvp("item", v); });
	}

}
