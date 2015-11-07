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
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>

#include <ctime>

#ifdef LV_PLATFORM_WINDOWS
#include <Windows.h>
#endif


namespace lv
{
	/*
	"""boost.random is thread-safe as long as you don't access
	any given object from two threads simultaneously.
	(Accessing two different objects is ok, as long as
	they don't share an engine). """
	*/

	template<typename T, typename Enabled = void>
	struct RandomTraits;

	template<typename T>
	struct RandomTraits<T, typename boost::enable_if<boost::is_integral<T> >::type>
	{
		typedef boost::mt19937	generator_type;

		typedef boost::uniform_int<T>	distribution_type;
	};


	template<typename T>
	struct RandomTraits<T, typename boost::enable_if<boost::is_float<T> >::type>
	{
		typedef boost::mt19937	generator_type;

		typedef boost::uniform_real<T>	distribution_type;
	};


	template<typename T = int, typename Traits = RandomTraits<T> >
	class Random
	{
		typedef typename Traits::generator_type		engine_type;
		typedef typename Traits::distribution_type	distribution_type;

		boost::variate_generator<engine_type, distribution_type> die_;

	public:

		typedef T result_type;

		Random(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
			: die_(engine_type(), distribution_type(min, max))
		{
#ifdef LV_PLATFORM_WINDOWS
			seed(clock() * rand() * GetTickCount());	// WINDOWS
#else
			seed(clock() * rand() * uint32(this));
#endif
		}

		void	seed(uint32 value)
		{
			die_.engine().seed(value);
		}
		
		result_type	operator() ()
		{
			return die_();
		}

		result_type	operator () (T min, T max)
		{
			distribution_type distri(min, max);
			return distri(die_.engine());
		}

		engine_type &	engine()
		{
			return die_.engine();
		}

		result_type	min() const
		{
			return die_.distribution().min();
		}

		result_type	max() const
		{
			return die_.distribution().max();
		}
		
	};
}



#endif