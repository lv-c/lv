// *********************************************************************
//  Serialization   version:  1.0   ¡¤  date: 04/06/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_SERIALIZATION_HPP
#define LV_SERIALIZATION_SERIALIZATION_HPP

#include <lv/IntType.hpp>
#include <lv/Serialization/Fwd.hpp>

#include <boost/serialization/array.hpp>
#include <boost/serialization/serialization.hpp>

namespace lv { namespace serialization {

	// arithmetic type or enum type
	template<typename T>
	struct is_primitive
	{
		static bool const value = 
			boost::type_traits::ice_or<
				boost::is_arithmetic<T>::value,
				boost::is_enum<T>::value
			>::value;
	};


} }


namespace boost { namespace serialization {

	template<>
	struct use_array_optimization<lv::OArchive>
	{
		template<class T>
		struct apply : boost::mpl::bool_<lv::serialization::is_primitive<T>::value> {};
	};

	template<>
	struct use_array_optimization<lv::IArchive>
	{
		template<class T>
		struct apply : boost::mpl::bool_<lv::serialization::is_primitive<T>::value> {};
	};

} }



#endif