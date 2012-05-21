// *********************************************************************
//  String   version:  1.0   ¡¤  date: 2012/05/20
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_INTERPROCESS_STRING_HPP
#define LV_SERIALIZATION_INTERPROCESS_STRING_HPP

#include <boost/interprocess/containers/string.hpp>

#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost { namespace serialization {

	template<class Archive, class C, class Tr, class Ax>
	void save(Archive & ar, interprocess::basic_string<C, Tr, Ax> const & t, unsigned int)
	{
		boost::serialization::stl::save_collection(ar, t);
	}

	template<class Archive, class C, class Tr, class Ax>
	void load(Archive & ar, interprocess::basic_string<C, Tr, Ax> & t, unsigned int)
	{
		boost::serialization::stl::load_collection<
			Archive,
			interprocess::basic_string<C, Tr, Ax>,
			boost::serialization::stl::archive_input_seq<
				Archive,
				interprocess::basic_string<C, Tr, Ax>
			>,
			boost::serialization::stl::reserve_imp<
				interprocess::basic_string<C, Tr, Ax>
			>
		>(ar, t);
	}


	template<class Archive, class C, class Tr, class Ax>
	void serialize(Archive & ar, interprocess::basic_string<C, Tr, Ax> & t, unsigned int version)
	{
		boost::serialization::split_free(ar, t, version);
	}

} }

#endif