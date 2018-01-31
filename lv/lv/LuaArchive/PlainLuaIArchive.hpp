// *********************************************************************
//  PlainLuaIArchive   version:  1.0   ¡¤  date: 2014/03/31
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/LuaArchive/PlainISerializer.hpp>

namespace lv
{
	class PlainLuaIArchive : boost::noncopyable
	{
		lua::archive::Parser	parser_;

	public:

		using is_loading = boost::mpl::true_;
		using is_saving = boost::mpl::false_;

		explicit PlainLuaIArchive(ConstBufferRef buf)
			: parser_(buf)
		{
		}

		template<class T>
		PlainLuaIArchive & operator >> (T & t)
		{
			lua::archive::load(parser_, t);
			return *this;
		}

		template<class T>
		PlainLuaIArchive & operator & (T & t)
		{
			return *this >> t;
		}
	};


	class IStreamFactory;

	template<class Archive>
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
