// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 2012/05/20
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/interprocess/containers/vector.hpp>

#include <boost/serialization/split_free.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/array.hpp>

#include <boost/mpl/bool.hpp>


namespace boost::serialization
{
	namespace detail
	{
		template<class T, class Ax>
		T * get_data(interprocess::vector<T, Ax> & v)
		{
			return v.empty() ? 0 : &(v[0]);
		}

		template<class T, class Ax>
		T const * get_data(interprocess::vector<T, Ax> const & v)
		{
			return v.empty() ? 0 : &(v[0]);
		}
	}

	// the default versions

	template<class Archive, class T, class Ax>
	void save(Archive & ar, interprocess::vector<T, Ax> const & t, unsigned int, mpl::false_)
	{
		boost::serialization::stl::save_collection(ar, t);
	}

	template<class Archive, class T, class Ax>
	void load(Archive & ar, interprocess::vector<T, Ax> & t, unsigned int, mpl::false_)
	{
		boost::serialization::stl::load_collection<
			Archive,
			interprocess::vector<T, Ax>,
			boost::serialization::stl::archive_input_seq<
				Archive, interprocess::vector<T, Ax>
			>,
			boost::serialization::stl::reserve_imp<interprocess::vector<T, Ax> >
		>(ar, t);
	}

	// the optimized versions

	template<class Archive, class T, class Ax>
	void save(Archive & ar, interprocess::vector<T, Ax> const & t, unsigned int, mpl::true_)
	{
		collection_size_type const count(t.size());
		ar << BOOST_SERIALIZATION_NVP(count);

		if (!t.empty())
		{
			ar << make_array(detail::get_data(t), t.size());
		}
	}

	template<class Archive, class T, class Ax>
	void load(Archive & ar, interprocess::vector<T, Ax> & t, unsigned int, mpl::true_)
	{
		collection_size_type count(t.size());
		ar >> BOOST_SERIALIZATION_NVP(count);

		t.resize(count);

		if (!t.empty())
		{
			ar >> make_array(detail::get_data(t), t.size());
		}
	}

	// dispatch to either default or optimized versions

	template<class Archive, class T, class Ax>
	void save(Archive & ar, interprocess::vector<T, Ax> const & t, unsigned int version)
	{
		using use_optimized = typename boost::serialization::use_array_optimization<Archive>::template apply<
			typename remove_const<T>::type
		>::type;

		save(ar, t, version, use_optimized());
	}

	template<class Archive, class T, class Ax>
	void load(Archive & ar, interprocess::vector<T, Ax> & t, unsigned int version)
	{
		using use_optimized = typename boost::serialization::use_array_optimization<Archive>::template apply<
			typename remove_const<T>::type
		>::type;

		load(ar, t, version, use_optimized());
	}

	// split

	template<class Archive, class T, class Ax>
	void serialize(Archive & ar, interprocess::vector<T, Ax> & t, unsigned int version)
	{
		boost::serialization::split_free(ar, t, version);
	}

}
