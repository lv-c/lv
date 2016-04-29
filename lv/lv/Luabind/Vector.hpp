// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 12/08/2007
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUABIND_VECTOR_HPP
#define LV_LUABIND_VECTOR_HPP

#include <lv/Luabind/Utility.hpp>

#include <vector>

#include <boost/type_traits/has_trivial_constructor.hpp>

#include <luabind/class.hpp>
#include <luabind/iterator_policy.hpp>

namespace lv { namespace lua {
	
	namespace detail
	{
		template<class T, bool Enable>
		struct bind_vector_constructor
		{
			static void bind(luabind::class_<T> & x)
			{
			}
		};

		template<class T>
		struct bind_vector_constructor<T, true>
		{
			static void bind(luabind::class_<T> & x)
			{
				x.def(luabind::constructor<typename T::size_type>());		// need default constructor
			}
		};
	}


	template<class T>
	void bind_vector(lua_State * L, char const* name)
	{
		typedef std::vector<T>	type;
		typedef typename type::const_reference	const_reference;
		typedef typename type::reference		reference;
		typedef typename type::size_type		size_type;

		using namespace luabind;

		class_<type> vec(name);
		vec
			.def(constructor<>(), return_stl_iterator)
			.def("size", &type::size)
			.def("empty", &type::empty)
			.def("clear", &type::clear)
			.def("capacity", &type::capacity)
			.def("reserve", &type::reserve)
			.def("at", (const_reference(type::*)(size_type) const)&type::at)
			.def("front", (const_reference(type::*)() const)&type::front)
			.def("back", (const_reference(type::*)() const)&type::back)
			.def("push_back", &type::push_back)
			.def("pop_back", &type::pop_back)
		;

		if (! is_primitive<T>::value)
		{
			vec
				.def("at", (reference(type::*)(size_type))&type::at)
				.def("front", (reference(type::*)())&type::front)
				.def("back", (reference(type::*)())&type::back)
			;
		}
		
		detail::bind_vector_constructor<type, boost::has_trivial_constructor<T>::value>::bind(vec);

		module(L)
		[
			vec
		];
	}
} }

#endif