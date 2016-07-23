// *********************************************************************
//  OArchiveWrapper   version:  1.0   ¡¤  date: 2016/07/23
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2016 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_OARCHIVEHOLDER_HPP
#define LV_SERIALIZATION_OARCHIVEHOLDER_HPP

#include <lv/Stream/OStreamFactory.hpp>


namespace lv
{
	template<typename Archive>
	class OArchiveWrapper
	{
		OStreamFactory::unique_pointer	raw_os_;

		Archive	ar_;

	public:

		OArchiveWrapper(OStreamFactory & factory, Buffer & buf)
			: raw_os_(factory.open(buf))
			, ar_(*raw_os_)
		{
		}

		Archive &	get()
		{
			return ar_;
		}

		void	flush()
		{
			raw_os_->flush();
		}
	};
}


#endif