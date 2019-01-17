// *********************************************************************
//  Tags   version:  1.0   ¡¤  date: 04/01/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>

#include <boost/range/value_type.hpp>


namespace lv::bstream
{
	// arithmetic type or enum type
	template<class T>
	struct is_primitive 
		: std::integral_constant<bool, std::is_arithmetic_v<T> || std::is_enum_v<T> >
	{
	};


	template<class T>
	constexpr bool	is_primitive_v = is_primitive<T>::value;


	// tags

	struct primitive_tag {};
	struct primitive_buffer_tag {};	// copyable in continuous memory (vector<PodType>, (w)string ...)
	struct range_tag {};			// list, boost::iterator_range, boost::sub_range ...
	struct unknown_tag {};


	template<class T, class = void>
	struct object_tag : unknown_tag {};

	template<class T>
	struct object_tag<T, std::enable_if_t<is_primitive_v<T> > >
		: primitive_tag
	{
	};

	template<class T, size_t N>
	struct object_tag<T[N], std::enable_if_t<is_primitive_v<T> > >
		: primitive_buffer_tag
	{
	};

	template<class T, size_t N>
	struct object_tag<T[N], std::enable_if_t<!is_primitive_v<T> > >
		: range_tag
	{
	};



	namespace detail
	{
		template<class T>
		struct is_range_value_primitive
			: is_primitive<typename boost::range_value<T>::type>
		{
		};

		template<class T>
		constexpr bool	is_contiguous_primitive_sequence_v =
			std::conjunction<
				std::is_base_of<
					container_adaptor::contiguous_sequence_tag,
					container_adaptor::container_category<T>
				>,
				is_range_value_primitive<T>
			>::value;
	}

	template<class T>
	struct object_tag<T, std::enable_if_t<detail::is_contiguous_primitive_sequence_v<T> > >
		: primitive_buffer_tag
	{
	};

	template<class T>
	struct object_tag<T, std::enable_if_t<container_adaptor::is_container_v<T>
		&& !detail::is_contiguous_primitive_sequence_v<T> > >
		: range_tag
	{
	};

}