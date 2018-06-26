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



	inline float	endian_switch(float v) noexcept
	{
		return reinterpret<float>(endian_switch(reinterpret<uint32_t>(v)));
	}

	inline double	endian_switch(double v) noexcept
	{
		return reinterpret<double>(endian_switch(reinterpret<uint64_t>(v)));
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
