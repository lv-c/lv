// *********************************************************************
//  LuaIArchive   version:  1.0   ��  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

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

		using is_loading = boost::mpl::true_;
		using is_saving = boost::mpl::false_;

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
		
		template<class T>
		LuaIArchive & operator >> (T && t)
		{
			load(t);
			return *this;
		}

		template<class T>
		LuaIArchive & operator & (T && t)
		{
			return *this >> std::forward<T>(t);
		}

	private:

		template<class T>
		void	load(T & t)
		{
			check_nvp(false);

			lua::archive::load_adl(table_[index_], t);
			index_++;
		}

		template<class T>
		void	load(boost::serialization::nvp<T> const & t)
		{
			check_nvp(true);

			lua::archive::load_adl(table_, t);
		}

		void	check_nvp(bool is_nvp)
		{
			if (!boost::indeterminate(nvp_))
			{
				BOOST_ASSERT(((bool)nvp_ == is_nvp) && "you are trying to mix nvp with non-nvp");
			}

			nvp_ = is_nvp;
		}
	};
}

