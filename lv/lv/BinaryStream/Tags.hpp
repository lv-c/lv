// *********************************************************************
//  Tags   version:  1.0   ¡¤  date: 04/01/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_TAGS_HPP
#define LV_BINARYSTREAM_TAGS_HPP

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>

namespace lv { namespace bstream {


	// arithmetic type or enum type
	template<typename T>
	struct is_primitive 
		: boost::mpl::or_<boost::is_arithmetic<T>, boost::is_enum<T> >
	{
	};


	// tags

#define DEFINE_tag(tag) \
	struct tag : boost::mpl::identity<tag> {}

	DEFINE_tag(primitive_tag);
	DEFINE_tag(primitive_buffer_tag);	// copyable in continuous memory (vector<PodType>, (w)string ...)
	DEFINE_tag(range_tag);		// list, boost::iterator_range, boost::sub_range ...
	DEFINE_tag(unknown_tag);

#undef DEFINE_tag

	template<typename T, class Enabled = void>
	struct object_tag : unknown_tag {};

	template<typename T>
	struct object_tag<T, typename boost::enable_if<is_primitive<T> >::type>
		: primitive_tag
	{
	};


} }

#endif