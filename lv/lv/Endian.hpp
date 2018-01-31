// *********************************************************************
//  Endian   version:  1.0   ¡¤  date: 08/05/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/IntType.hpp>

#include <boost/endian/conversion.hpp>


namespace lv
{
	template<class T>
	inline std::enable_if_t<!std::is_enum_v<T>, T>	endian_switch(T v) noexcept
	{
		return boost::endian::endian_reverse(v);
	}

	template<class T>
	inline std::enable_if_t<std::is_enum_v<T>, T>	endian_switch(T v) noexcept
	{
		return static_cast<T>(endian_switch(static_cast<std::underlying_type_t<T>>(v)));
	}


	// http://www.boost.org/doc/libs/1_65_1/libs/endian/doc/index.html#FAQ
	// "Why is there no floating point support? 
	// ...Even with those limitations, support for floating point types was not reliable and was removed.
	// For example, simply reversing the endianness of a floating point number can result in a signaling-NAN. "

	namespace detail
	{
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

	inline float	endian_switch(float v) noexcept
	{
		return detail::reinterpret<float>(endian_switch(detail::reinterpret<uint32>(v)));
	}

	inline double	endian_switch(double v) noexcept
	{
		return detail::reinterpret<double>(endian_switch(detail::reinterpret<uint64>(v)));
	}


	constexpr	bool	little_endian() noexcept
	{
#ifdef BOOST_LITTLE_ENDIAN
		return true;
#else
		return false;
#endif
	}

	constexpr	bool	big_endian() noexcept
	{
		return !little_endian();
	}
}
