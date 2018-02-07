// *********************************************************************
//  IntType   version:  1.0   ��  date: 08/05/2008
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
	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;

	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;



	// size to type

	template<std::size_t size>
	struct IntType;

	template<>
	struct IntType<1>
	{
		using type = int8;
	};

	template<>
	struct IntType<2>
	{
		using type = int16;
	};

	template<>
	struct IntType<4>
	{
		using type = int32;
	};

	template<>
	struct IntType<8>
	{
		using type = int64;
	};


	template<std::size_t size>
	struct UIntType : std::make_unsigned<typename IntType<size>::type>
	{
	};



	// for ( streaming
	template<class T>
	struct WidenInt
	{
		static_assert(std::is_arithmetic_v<T>);

		using type = T;
	};

	template<>
	struct WidenInt<int8>		// signed char
	{
		using type = int32;
	};

	template<>
	struct WidenInt<uint8>		// unsigned char
	{
		using type = uint32;
	};

	template<>
	struct WidenInt<char>		// plain char
	{
		using type = int32;
	};

	
	template<class T>
	typename WidenInt<T>::type widen_int(T t)
	{
		return t;
	}


	//
	template<class T>
	inline int	sign(T val) noexcept
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
}

