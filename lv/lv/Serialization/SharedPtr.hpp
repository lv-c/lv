// *********************************************************************
//  SharedPtr   version:  1.0   ¡¤  date: 2013/01/03
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_SHAREDPTR_HPP
#define LV_SERIALIZATION_SHAREDPTR_HPP

#include <memory>


namespace lv { namespace serialization {

	template<class Archive, typename T>
	void	save(Archive & ar, std::shared_ptr<T> const & t, Overload)
	{
		bool exist(t);
		ar << exist;

		if (exist)
		{
			ar << *t;
		}
	}

	template<class Archive, typename T>
	void	load(Archive & ar, std::shared_ptr<T> & t, Overload)
	{
		bool exist;
		ar >> exist;

		if (exist)
		{
			t = std::make_shared<T>();
			ar >> *t;
		}
	}

} }

#endif
