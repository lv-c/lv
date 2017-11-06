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


namespace boost::serialization
{
	template<class Archive, class C, class Tr, class Ax>
	void save(Archive & ar, interprocess::basic_string<C, Tr, Ax> const & t, unsigned int)
	{
		collection_size_type const count(t.size());
		ar << BOOST_SERIALIZATION_NVP(count);

		if (! t.empty())
		{
			ar << make_array(t.data(), t.size());
		}
	}

	template<class Archive, class C, class Tr, class Ax>
	void load(Archive & ar, interprocess::basic_string<C, Tr, Ax> & t, unsigned int)
	{
		collection_size_type count(t.size());
		ar >> BOOST_SERIALIZATION_NVP(count);
		t.resize(count);

		if (! t.empty())
		{
			// TODO: t.data(): <b>Requires< / b>: The program shall not alter any of the values stored in the character array.
			// So it may be wrong here.
			ar >> make_array(const_cast<C *>(t.data()), t.size());
		}
	}

	template<class Archive, class C, class Tr, class Ax>
	void serialize(Archive & ar, interprocess::basic_string<C, Tr, Ax> & t, unsigned int version)
	{
		boost::serialization::split_free(ar, t, version);
	}

}

#endif