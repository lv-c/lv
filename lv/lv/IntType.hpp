// *********************************************************************
//  IntType   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#pragma once

#include <cstdint>
#include <type_traits>


namespace lv
{
	// size to type

	template<std::size_t size>
	struct IntType;

	template<>
	struct IntType<1>
	{
		using type = int8_t;
	};

	template<>
	struct IntType<2>
	{
		using type = int16_t;
	};

	template<>
	struct IntType<4>
	{
		using type = int32_t;
	};

	template<>
	struct IntType<8>
	{
		using type = int64_t;
	};


	template<std::size_t size>
	struct UIntType : std::make_unsigned<typename IntType<size>::type>
	{
	};



	// for streaming
	template<class T>
	struct WidenInt
	{
		static_assert(std::is_arithmetic_v<T>);

		using type = T;
	};

	template<>
	struct WidenInt<int8_t>		// signed char
	{
		using type = int32_t;
	};

	template<>
	struct WidenInt<uint8_t>	// unsigned char
	{
		using type = uint32_t;
	};

	template<>
	struct WidenInt<char>		// plain char
	{
		using type = int32_t;
	};

	
	template<class T>
	typename WidenInt<T>::type widen_int(T t)
	{
		return t;
	}


	//
	template<class T>
	inline int	sign(T val)
	{
		return (T(0) < val) - (val < T(0));
	}


	template<class T, class U>
	inline T	reinterpret(U v) noexcept
	{
		static_assert(sizeof(T) == sizeof(U));

		union {
			U	u;
			T	t;
		} obj;

		obj.u = v;
		return obj.t;
	}


	//
	template<char c0, char c1, char c2, char c3>
	struct fourcc
	{
		static constexpr uint32_t	value = (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
	};


	template<char c0, char c1, char c2, char c3>
	constexpr uint32_t fourcc_v = fourcc<c0, c1, c2, c3>::value;

}

