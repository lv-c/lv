// *********************************************************************
//  NonintrusiveOptional   version:  1.0   ¡¤  date: 2014/03/09
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/serialization/nvp.hpp>


namespace lv::serialization
{
	template<class T>
	class NonintrusiveOptional
	{
		T &	val_;

	public:

		explicit NonintrusiveOptional(T & val)
			: val_(val)
		{
		}

		T &	value() const
		{
			return val_;
		}
	};


	template<class T>
	inline NonintrusiveOptional<T>	make_optional(T & val)
	{
		return NonintrusiveOptional<T>(val);
	}


#define	LV_SERIALIZATION_OPTIONAL_NVP(name)				\
	boost::serialization::make_nvp(#name, lv::serialization::make_optional(name))

}