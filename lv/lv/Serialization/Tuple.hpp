// *********************************************************************
//  Tuple   version:  1.0   ¡¤  date: 12/15/2008
//  --------------------------------------------------------------------
//	Serializing boost.tuple using boost.serialization
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#if !BOOST_PP_IS_ITERATING

#ifndef LV_PACKET_TUPLE_HPP
#define LV_PACKET_TUPLE_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace lv { namespace detail {

	template<class Archive>
	class Serializer
	{
		Archive & ar_;
	public:
		Serializer(Archive & ar) : ar_(ar) {}

		template<typename T>
		void operator () (T & t) const
		{
			ar_ & boost::serialization::make_nvp("item", t);
		}
	};

} }

namespace boost { namespace serialization {
	
	
#ifndef	LV_TUPLE_SERIALIZATION_ARITY
#	define LV_TUPLE_SERIALIZATION_ARITY	5
#endif

#	define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_TUPLE_SERIALIZATION_ARITY, <lv/Serialization/Tuple.hpp>))
#	include BOOST_PP_ITERATE()

} }


#endif

#else

#define n BOOST_PP_ITERATION()

template<class Archive BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)>
void serialize(Archive & ar, boost::tuple<BOOST_PP_ENUM_PARAMS(n, T)> & t,
			   unsigned int const)
{
	boost::fusion::for_each(t, lv::detail::Serializer<Archive>(ar));
}

#undef n

#endif