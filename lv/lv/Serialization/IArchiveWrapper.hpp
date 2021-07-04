// *********************************************************************
//  IArchiveWrapper   version:  1.0   ¡¤  date: 2014/04/01
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Stream/IStreamFactory.hpp>


namespace lv
{
	template<class Archive>
	class IArchiveWrapper
	{
		IStreamFactory::unique_pointer	raw_is_;

		Archive	ar_;

	public:

		IArchiveWrapper(IStreamFactory & factory, ConstBufferRef buf)
			: raw_is_(factory.open(buf))
			, ar_(*raw_is_)
		{
		}

		Archive &	get()
		{
			return ar_;
		}
	};
}