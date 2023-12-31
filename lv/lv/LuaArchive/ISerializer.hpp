// *********************************************************************
//  ISerializer   version:  1.0   ��  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/LuaArchive/Fwd.hpp>
#include <lv/LuaArchive/Tags.hpp>
#include <lv/LuaArchive/Common.hpp>
#include <lv/ContainerAdaptor/Operations.hpp>
#include <lv/Ensure.hpp>

#include <luabind/object.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/value_type.hpp>

#include <stdexcept>


namespace lv::lua::archive
{

	class UnmatchedLuaType : public std::runtime_error
	{
	public:
		UnmatchedLuaType(int given, int expected)
			: std::runtime_error(std::string("unmatched lua type: ") +
				lua_typename(nullptr, expected) + " expected, " +
				lua_typename(nullptr, given) + " given.")
		{
		}

		explicit UnmatchedLuaType(std::string const & msg)
			: std::runtime_error(msg)
		{
		}
	};
	
	
	inline void	expect_obj_type(luabind::object const & obj, int expected)
	{
		int given = luabind::type(obj);
		LV_ENSURE(given == expected, UnmatchedLuaType(given, expected));
	}


	template<class T>
	void	load_adl(luabind::object const & obj, T & t);

	template<class T>
	void	load_item_adl(luabind::iterator const & it, T & t);


	namespace detail
	{
		template<class T>
		void	load_impl(luabind::object const & obj, T & t, unknown_tag)
		{
			LuaIArchive ia(obj);
			unsigned int version = DefaultVersion;

			luabind::object version_obj = obj[VersionKey];
			if (version_obj)
			{
				version = luabind::object_cast<unsigned int>(version_obj);
			}

			boost::serialization::serialize_adl(ia, t, version);
		}

		// primitive_tag
		template<class T>
		struct primitive_type : boost::mpl::int_<LUA_TNUMBER> {};

		template<>
		struct primitive_type<bool> : boost::mpl::int_<LUA_TBOOLEAN> {};

		template<size_t N>
		struct primitive_type<char[N]> : boost::mpl::int_<LUA_TSTRING> {};

		template<>
		struct primitive_type<std::string> : boost::mpl::int_<LUA_TSTRING> {};


		template<size_t N>
		void	load_impl(luabind::object const & obj, char (&t)[N], primitive_tag)
		{
			expect_obj_type(obj, primitive_type<char[N]>::value);

			lua_State * L = obj.interpreter();
			obj.push(L);
			luabind::detail::stack_pop pop(L, 1);

			int index = -1;
			size_t size = lua_rawlen(L, index);
			LV_ENSURE(size < N, std::overflow_error("string buffer too small"));

			std::copy_n(lua_tostring(L, index), size, t);
			t[size] = '\0';
		}

		template<class T>
		void	load_impl(luabind::object const & obj, T & t, primitive_tag)
		{
			expect_obj_type(obj, primitive_type<T>::value);

			t = luabind::object_cast<T>(obj);
		}

		// enum_tag
		template<class T>
		void	load_impl(luabind::object const & obj, T & t, enum_tag)
		{
			int v;
			archive::load_adl(obj, v);

			t = static_cast<T>(v);
		}

		// sequence_tag
		inline bool	is_version_key(luabind::object const & obj)
		{
			if (obj && luabind::type(obj) == LUA_TSTRING)
			{
				return (luabind::object_cast<std::string>(obj) == VersionKey);
			}
			
			return false;
		}

		template<class T>
		void	load_impl(luabind::object const & obj, T & t, container_adaptor::container_tag)
		{
			expect_obj_type(obj, LUA_TTABLE);

			lv::clear(t);
			for (luabind::iterator it(obj), end; it != end; ++it)
			{
				BOOST_ASSERT(!is_version_key(it.key()) && "you shouldn't place a version key here");

				typename boost::range_value<T>::type item;

				load_item_adl(it, item);
				lv::push(t, std::move(item));
			}
		}

		// utility
		template<class K, class V>
		void	load_key_value(luabind::iterator const & it, K & key, V & value)
		{
			load_adl(it.key(), key);
			load_adl(*it, value);
		}
	}

	// adl ~~
	template<class T>
	class Ref
	{
		T * t_;
	public:

		Ref(T & t) : t_(&t) {}

		operator T & () const { return *t_; }
	};

	template<class T>
	void	load(luabind::object const & obj, T & t)
	{
		detail::load_impl(obj, t, object_tag<T>());
	}

	template<class T>
	void	load_adl(luabind::object const & obj, T & t)
	{
		load(Ref<luabind::object const>(obj), t);
	}

	template<class T>
	void	load(luabind::object const & obj, boost::serialization::nvp<T> const & t)
	{
		try
		{
			load_adl(obj[t.name()], t.value());
		}
		catch (UnmatchedLuaType const & ex)
		{
			throw UnmatchedLuaType(t.name() + std::string(" -> ") + ex.what());
		}
	}

	// load items of a container. you should overload this function if the item
	// needs both the key and the value, such as std::pair
	template<class T>
	void	load_item(luabind::iterator const & it, T & t)
	{
		load_adl(*it, t);
	}

	template<class T>
	void	load_item_adl(luabind::iterator const & it, T & t)
	{
		load_item(Ref<luabind::iterator const>(it), t);
	}
}
