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

	namespace detail
	{
		template<class Archive, class T>
		void	save_smart_ptr(Archive & ar, T const & t)
		{
			bool exist(t);
			ar << exist;

			if (exist)
			{
				ar << *t;
			}
		}

		template<class Archive, class T, class F>
		void	load_smart_ptr(Archive & ar, T & t, F creator)
		{
			bool exist;
			ar >> exist;

			if (exist)
			{
				t = creator();
				ar >> *t;
			}
		}
	}

	template<class Archive, class T>
	void	save(Archive & ar, std::shared_ptr<T> const & t, Overload)
	{
		detail::save_smart_ptr(ar, t);
	}

	template<class Archive, class T>
	void	save(Archive & ar, std::unique_ptr<T> const & t, Overload)
	{
		detail::save_smart_ptr(ar, t);
	}

	//
	template<class Archive, class T>
	void	load(Archive & ar, std::shared_ptr<T> & t, Overload)
	{
		detail::load_smart_ptr(ar, t, &std::make_shared<T>);
	}

	template<class Archive, class T>
	void	load(Archive & ar, std::unique_ptr<T> & t, Overload)
	{
		detail::load_smart_ptr(ar, t, &std::make_unique<T>);
	}

} }

#endif
