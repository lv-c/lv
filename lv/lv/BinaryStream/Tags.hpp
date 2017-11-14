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

#include <boost/mpl/identity.hpp>

#include <type_traits>


namespace lv::bstream
{
	// arithmetic type or enum type
	template<typename T>
	struct is_primitive 
		: std::integral_constant<bool, std::is_arithmetic_v<T> || std::is_enum_v<T> >
	{
	};


	template<typename T>
	constexpr bool	is_primitive_v = is_primitive<T>::value;


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
	struct object_tag<T, std::enable_if_t<is_primitive_v<T> > >
		: primitive_tag
	{
	};


	template<typename T, class Enabled = void>
	using object_tag_t = typename object_tag<T, Enabled>::type;

}

#endif