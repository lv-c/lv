// *********************************************************************
//  PlainLuaIArchive   version:  1.0   ¡¤  date: 2014/03/31
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_PLAINLUAIARCHIVE_HPP
#define LV_LUAARCHIVE_PLAINLUAIARCHIVE_HPP

#include <lv/LuaArchive/PlainISerializer.hpp>

namespace lv
{
	class PlainLuaIArchive : boost::noncopyable
	{
		lua::archive::Parser	parser_;

	public:

		typedef boost::mpl::true_	is_loading;
		typedef boost::mpl::false_	is_saving;

		explicit PlainLuaIArchive(ConstBufferRef buf)
			: parser_(buf)
		{
		}

		template<typename T>
		PlainLuaIArchive & operator >> (T & t)
		{
			lua::archive::load(parser_, t);
			return *this;
		}

		template<typename T>
		PlainLuaIArchive & operator & (T & t)
		{
			return *this >> t;
		}
	};


	class IStreamFactory;

	template<typename Archive>
	class IArchiveWrapper;

	template<>
	class IArchiveWrapper<PlainLuaIArchive>
	{
		PlainLuaIArchive	ar_;

	public:

		IArchiveWrapper(IStreamFactory &, ConstBufferRef const & buf)
			: ar_(buf)
		{
		}

		PlainLuaIArchive &	get()
		{
			return ar_;
		}

	};
}


#endif