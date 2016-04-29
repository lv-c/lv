// *********************************************************************
//  LuaIArchive   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_LUAIARCHIVE_HPP
#define LV_LUAARCHIVE_LUAIARCHIVE_HPP

#include <lv/LuaArchive/Fwd.hpp>
#include <lv/LuaArchive/ISerializer.hpp>

#include <boost/noncopyable.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/serialization/nvp.hpp>

namespace lv
{
	class LuaIArchive : boost::noncopyable
	{
		luabind::object	table_;

		size_t	index_;

		boost::tribool	nvp_;

	public:

		typedef boost::mpl::true_	is_loading;
		typedef boost::mpl::false_	is_saving;

		explicit LuaIArchive(luabind::object const & table)
			: index_(1)
			, nvp_(boost::indeterminate)
		{
			set_table(table);
		}

		void	set_table(luabind::object const & table)
		{
			this->table_ = table;

			if (table.is_valid())
			{
				lua::archive::expect_obj_type(table_, LUA_TTABLE);
			}
		}
		
		template<typename T>
		LuaIArchive & operator >> (T & t)
		{
			load(t);
			return *this;
		}

		template<typename T>
		LuaIArchive & operator & (T & t)
		{
			return *this >> t;
		}

	private:

		template<typename T>
		void	load(T & t)
		{
			check_nvp(false);

			lua::archive::load(table_[index_], t);
			index_++;
		}

		template<typename T>
		void	load(boost::serialization::nvp<T> const & t)
		{
			check_nvp(true);

			lua::archive::load(table_, t);
		}

		void	check_nvp(bool is_nvp)
		{
			if (! boost::indeterminate(nvp_))
			{
				BOOST_ASSERT(((bool)nvp_ == is_nvp) && "you are trying to mix nvp with non-nvp");
			}

			nvp_ = is_nvp;
		}
	};
}


#endif