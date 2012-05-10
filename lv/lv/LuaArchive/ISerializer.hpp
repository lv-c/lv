// *********************************************************************
//  ISerializer   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_ISERIALIZER_HPP
#define LV_LUAARCHIVE_ISERIALIZER_HPP

#include <lv/LuaArchive/Tags.hpp>
#include <lv/LuaArchive/Common.hpp>
#include <lv/ContainerAdaptor/Adaptor.hpp>

#include <luabind/object.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/value_type.hpp>

#include <stdexcept>

namespace lv { namespace lua { namespace archive {

	class UnmatchedLuaType : public std::runtime_error
	{
	public:
		UnmatchedLuaType(int given, int expected)
			: std::runtime_error(std::string("unmatched lua type:") +
				lua_typename(NULL, expected) + " expected, " +
				lua_typename(NULL, given) + " given.")
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
		if(given != expected)
		{
			throw UnmatchedLuaType(given, expected);
		}
	}


	template<typename T>
	void	load(luabind::object const & obj, T & t);

	// load items of a container. you should overload this function if the item
	// needs both the key and the value, such as std::pair
	template<typename T>
	void	load_item(luabind::iterator const & it, T & t);


	namespace detail
	{		
		// unknown_tag
		class IArchiveProxy : boost::noncopyable
		{
			luabind::object const & obj_;

			size_t	index_;

			boost::tribool	nvp_;

		public:

			typedef boost::mpl::true_	is_loading;
			typedef boost::mpl::false_	is_saving;

			IArchiveProxy(luabind::object const & obj)
				: obj_(obj)
				, index_(1)
				, nvp_(boost::indeterminate)
			{
				expect_obj_type(obj, LUA_TTABLE);
			}


			template<typename T>
			IArchiveProxy & operator >> (T & t)
			{
				this->load(t);

				return *this;
			}

			template<typename T>	
			IArchiveProxy & operator & (T & t)
			{
				return *this >> t;
			}

		private:

			template<typename T>
			void	load(T & t)
			{
				check_nvp(false);

				archive::load(obj_[index_], t);
				index_++;
			}

			template<typename T>
			void	load(boost::serialization::nvp<T> const & t)
			{
				check_nvp(true);

				archive::load(obj_, t);
			}


			void	check_nvp(bool is_nvp)
			{
				if(! boost::indeterminate(nvp_))
				{
					BOOST_ASSERT(((bool)nvp_ == is_nvp) && "you are trying to mix nvp with non-nvp");
				}

				nvp_ = is_nvp;
			}
		};


		template<typename T>
		void	load_impl(luabind::object const & obj, T & t, unknown_tag)
		{
			IArchiveProxy proxy(obj);
			unsigned int version = DefaultVersion;

			luabind::object version_obj = obj[VersionKey];
			if(version_obj)
			{
				version = luabind::object_cast<unsigned int>(version_obj);
			}

			boost::serialization::serialize(proxy, t, version);
		}

		// primitive_tag
		template<typename T>
		struct primitive_type : boost::mpl::int_<LUA_TNUMBER> {};

		template<>
		struct primitive_type<bool> : boost::mpl::int_<LUA_TBOOLEAN> {};

		template<>
		struct primitive_type<std::string> : boost::mpl::int_<LUA_TSTRING> {};


		template<typename T>
		void	load_impl(luabind::object const & obj, T & t, primitive_tag)
		{
			expect_obj_type(obj, primitive_type<T>::value);

			t = luabind::object_cast<T>(obj);
		}

		// enum_tag
		template<typename T>
		void	load_impl(luabind::object const & obj, T & t, enum_tag)
		{
			int v;
			archive::load(obj, v);

			t = static_cast<T>(v);
		}

		// sequence_tag
		inline bool	is_version_key(luabind::object const & obj)
		{
			if(obj && luabind::type(obj) == LUA_TSTRING)
			{
				return (luabind::object_cast<std::string>(obj) == VersionKey);
			}
			
			return false;
		}

		template<typename T>
		void	load_impl(luabind::object const & obj, T & t, sequence_tag)
		{
			expect_obj_type(obj, LUA_TTABLE);

			lv::clear(t);
			for(luabind::iterator it(obj), end; it != end; ++it)
			{
				BOOST_ASSERT(! is_version_key(it.key()) && "you shouldn't place a version key here");

				typename boost::range_value<T>::type item;

				load_item(it, item);
				lv::insert(t, item);
			}
		}

		// utility
		template<typename K, typename V>
		void	load_key_value(luabind::iterator const & it, K & key, V & value)
		{
			load(it.key(), key);
			load(*it, value);
		}
	}


	template<typename T>
	void	load(luabind::object const & obj, T & t)
	{
		detail::load_impl(obj, t, object_tag<T>::type());
	}

	template<typename T>
	void	load(luabind::object const & obj, boost::serialization::nvp<T> const & t)
	{
		try
		{
			load(obj[t.name()], t.value());
		}
		catch(UnmatchedLuaType const & ex)
		{
			throw UnmatchedLuaType(t.name() + string(" -> ") + ex.what());
		}
	}

	template<typename T>
	void	load_item(luabind::iterator const & it, T & t)
	{
		load(*it, t);
	}

} } }

#endif
