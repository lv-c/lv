// *********************************************************************
//  Fusion   version:  1.0   ¡¤  date: 11/27/2008
//  --------------------------------------------------------------------
//  mpl sequence to fusion::cons and fusion::vector
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FUSION_HPP
#define LV_FUSION_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/reverse_fold.hpp>
#include <boost/mpl/is_sequence.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/cons.hpp>

namespace lv
{
	template<class T>
	struct MplToFusionCons
	{
		typedef typename boost::mpl::reverse_fold<T, boost::fusion::nil, 
			boost::fusion::cons<boost::mpl::_2, boost::mpl::_1> >::type type;
	};
	
	template<class T>
	struct MplToFusionVec
	{
		typedef typename boost::fusion::result_of::as_vector<typename 
			MplToFusionCons<T>::type>::type type;
	};

}

#endif