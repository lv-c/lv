// *********************************************************************
//  IArchiveCreator   version:  1.0   ¡¤  date: 2014/04/01
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_IARCHIVEHOLDER_HPP
#define LV_SERIALIZATION_IARCHIVEHOLDER_HPP

#include <lv/Stream/IStreamFactory.hpp>

namespace lv
{
	template<typename Archive>
	class IArchiveCreator
	{
		IStreamPtr	raw_is_;

		Archive	ar_;

	public:

		IArchiveCreator(IStreamFactory & factory, ConstBufferRef const & buf)
			: raw_is_(factory.open(buf))
			, ar_(*raw_is_)
		{
		}

		Archive &	archive()
		{
			return ar_;
		}
	};
}

#endif