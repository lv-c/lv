// *********************************************************************
//  RemoveSecond   version:  1.0   ¡¤  date: 12/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/erase.hpp>


namespace lv::detail
{
	namespace mpl = boost::mpl;

	template<class T>
	struct RemoveSecond
	{
		using type = typename mpl::erase<T, 
			typename mpl::next<
				typename mpl::begin<T>::type
			>::type
		>::type;
	};

}