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

namespace lv { namespace lua { namespace archive {

	class LuaOArchive
	{
		std::ostream &	os_;

	public:

		typedef boost::mpl::true_	is_saving;
		typedef boost::mpl::false_	is_loading;

		explicit LuaOArchive(std::ostream & os)
			: os_(os)
		{
		}

		template<typename T>
		LuaOArchive & operator << (T const & t)
		{
			archive::save(os_, t, 0);
			os_ << std::endl << std::endl;

			return *this;
		}

		template<typename T>
		LuaOArchive & operator & (T const & t)
		{
			return *this << t;
		}
	};

} } }


#endif