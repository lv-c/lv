// *********************************************************************
//  Tags   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/mpl/identity.hpp>

#include <string>
#include <type_traits>


namespace lv::lua::archive
{

#define DEFINE_tag(tag) \
	struct tag : boost::mpl::identity<tag> {}

	DEFINE_tag(primitive_tag);
	DEFINE_tag(enum_tag);
	DEFINE_tag(sequence_tag);
	DEFINE_tag(unknown_tag);

#undef DEFINE_tag


	// arithmetic type or enum type or string type
	template<class T>
	struct is_primitive : std::is_arithmetic<T>
	{
	};

	template<>
	struct is_primitive<std::string> : std::true_type
	{
	};

	template<>
	struct is_primitive<char const *> : std::true_type
	{
	};

	template<int N>
	struct is_primitive<char[N]> : std::true_type
	{
	};

	template<int N>
	struct is_primitive<char const [N]> : std::true_type
	{
	};


	template<class T>
	constexpr bool	is_primitive_v = is_primitive<T>::value;



	template<class T, class Enabled = void>
	struct object_tag : unknown_tag {};

	template<class T>
	struct object_tag<T, std::enable_if_t<is_primitive_v<T> > >
		: primitive_tag
	{
	};

	template<class T>
	struct object_tag<T, std::enable_if_t<std::is_enum_v<T> > >
		: enum_tag
	{
	};


	template<class T, class Enabled = void>
	using object_tag_t = typename object_tag<T, Enabled>::type;

}