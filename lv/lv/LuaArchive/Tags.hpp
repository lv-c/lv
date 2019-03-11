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

#include <lv/ContainerAdaptor/Tags.hpp>

#include <string>
#include <type_traits>


namespace lv::lua::archive
{

	struct primitive_tag {};
	struct enum_tag {};
	struct unknown_tag {};


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



	template<class T, class = void>
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

	template<class T>
	struct object_tag<T, std::enable_if_t<
		container_adaptor::is_container_v<T> && !is_primitive_v<T> > >
		: container_adaptor::container_category<T>
	{
	};

}