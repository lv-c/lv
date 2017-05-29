// *********************************************************************
//  RemoveSecond   version:  1.0   ¡¤  date: 12/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_REMOVE_SECOND_HPP
#define LV_REMOVE_SECOND_HPP

#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/erase.hpp>

namespace lv { namespace detail {

	namespace mpl = boost::mpl;

	template<class T>
	struct RemoveSecond
	{
		typedef typename mpl::erase<T, 
			typename mpl::next<
				typename mpl::begin<T>::type
			>::type
		>::type type;
	};

} }

#endif