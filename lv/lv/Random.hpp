// *********************************************************************
//  Random   version:  1.0   ·  date: 04/27/2009
//  --------------------------------------------------------------------
//  简化 boost.random 的使用， 不需要很简单的应用也要写一堆东西
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RANDOM_HPP
#define LV_RANDOM_HPP

#include <limits>

#include <lv/IntType.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>


namespace lv
{
	template<typename T = int>
	class Random
	{
		boost::variate_generator<boost::mt19937, boost::uniform_int<T> > die_;

	public:

		typedef T result_type;

		Random(uint32 seed = 5489, T min = std::numeric_limits<T>::min(), 
			T max = std::numeric_limits<T>::max())
			: die_(boost::mt19937(seed), boost::uniform_int<T>(min, max))
		{
		}

		void	seed(uint32 value)
		{
			die_.engine().seed(value);
		}

		
		result_type	operator() ()
		{
			return die_();
		}
		
	};
}



#endif // LV_RANDOM_HPP