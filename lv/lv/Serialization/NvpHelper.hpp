// *********************************************************************
//  NvpHelper   version:  1.0   ¡¤  date: 09/26/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_NVPHELPER_HPP
#define LV_SERIALIZATION_NVPHELPER_HPP

#include <boost/serialization/nvp.hpp>

namespace lv
{
	template<class Archive>
	class NvpHelper
	{
		Archive &	ar_;

	public:
		
		explicit NvpHelper(Archive & ar)
			: ar_(ar)
		{
		}

		template<typename T>
		NvpHelper const & operator () (char const * name, T & t) const
		{
			ar_ & boost::serialization::make_nvp(name, t);
			return *this;
		}
	};
}


#endif