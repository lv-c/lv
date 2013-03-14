// *********************************************************************
//  IntType   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_INTTYPE_HPP
#define LV_INTTYPE_HPP

#include <boost/cstdint.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/mpl/assert.hpp>

namespace lv
{
	typedef boost::int8_t	int8;
	typedef boost::int16_t	int16;
	typedef boost::int32_t	int32;
	typedef boost::int64_t	int64;

	typedef boost::uint8_t	uint8;
	typedef boost::uint16_t	uint16;
	typedef boost::uint32_t	uint32;
	typedef boost::uint64_t	uint64;



	// size to type

	template<size_t size>
	struct IntType;

	template<>
	struct IntType<1>
	{
		typedef	int8	type;
	};

	template<>
	struct IntType<2>
	{
		typedef int16	type;
	};

	template<>
	struct IntType<4>
	{
		typedef int32	type;
	};

	template<>
	struct IntType<8>
	{
		typedef int64	type;
	};


	template<size_t size>
	struct UIntType : boost::make_unsigned<typename IntType<size>::type>
	{
	};



	// for streaming
	template<typename T>
	struct WidenInt
	{
		BOOST_MPL_ASSERT((boost::is_integral<T>));

		typedef	typename T	type;
	};

	template<>
	struct WidenInt<int8>		// signed char
	{
		typedef int32 type;
	};

	template<>
	struct WidenInt<uint8>		// unsigned char
	{
		typedef uint32 type;
	};

	template<>
	struct WidenInt<char>		// plain char
	{
		typedef int32 type;
	};

	
	template<typename T>
	typename WidenInt<T>::type widen_int(T t)
	{
		return t;
	}


	//
	template<typename T>
	int	sign(T val)
	{
		return (T(0) < val) - (val < T(0));
	}
}


#endif