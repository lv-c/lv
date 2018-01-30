// *********************************************************************
//  LuaOArchive   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_LUAOARCHIVE_HPP
#define LV_LUAARCHIVE_LUAOARCHIVE_HPP

#include <lv/LuaArchive/Fwd.hpp>
#include <lv/LuaArchive/OSerializer.hpp>

#include <boost/noncopyable.hpp>

namespace lv
{

	class LuaOArchive : boost::noncopyable
	{
		std::ostream &	os_;

		std::string		spliter_;

		bool	first_time_;

	public:

		using is_saving = boost::mpl::true_;
		using is_loading = boost::mpl::false_;

		LuaOArchive(std::ostream & os, std::string const & spliter = ", ")
			: os_(os)
			, spliter_(spliter)
			, first_time_(true)
		{
		}

		template<class T>
		LuaOArchive & operator << (T const & t)
		{
			if (!first_time_)
			{
				os_ << spliter_;
			}
			else
			{
				first_time_ = false;
			}

			lua::archive::save_adl(os_, t, 0);
			return *this;
		}

		template<class T>
		LuaOArchive & operator & (T const & t)
		{
			return *this << t;
		}
	};

}


#endif
