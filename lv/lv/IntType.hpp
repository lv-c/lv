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

#include <cstdint>
#include <type_traits>


namespace lv
{
	typedef std::int8_t		int8;
	typedef std::int16_t	int16;
	typedef std::int32_t	int32;
	typedef std::int64_t	int64;

	typedef std::uint8_t	uint8;
	typedef std::uint16_t	uint16;
	typedef std::uint32_t	uint32;
	typedef std::uint64_t	uint64;



	// size to type

	template<std::size_t size>
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


	template<std::size_t size>
	struct UIntType : std::make_unsigned<typename IntType<size>::type>
	{
	};



	// for ( streaming
	template<typename T>
	struct WidenInt
	{
		static_assert(std::is_arithmetic_v<T>);

		typedef	T	type;
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
