// *********************************************************************
//  Tags   version:  1.0   ��  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_TAGS_HPP
#define LV_LUAARCHIVE_TAGS_HPP

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

#include <string>

namespace lv { namespace lua { namespace archive {

#define DEFINE_tag(tag) \
	struct tag : boost::mpl::identity<tag> {}

	DEFINE_tag(primitive_tag);
	DEFINE_tag(sequence_tag);
	DEFINE_tag(unordered_tag);
	DEFINE_tag(unknown_tag);

#undef DEFINE_tag


	// arithmetic type or enum type or string type
	template<typename T>
	struct is_primitive
	{
		static bool const value = 
			boost::type_traits::ice_or<
				boost::is_arithmetic<T>::value,
				boost::is_enum<T>::value
			>::value;
	};

	template<>
	struct is_primitive<std::string> : boost::mpl::true_
	{
	};


	template<typename T, class Enabled = void>
	struct object_tag : unknown_tag {};

	template<typename T>
	struct object_tag<T, typename boost::enable_if<is_primitive<T> >::type>
		: primitive_tag
	{
	};

} } }

#endif