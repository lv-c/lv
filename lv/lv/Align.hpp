// *********************************************************************
//  Align   version:  1.0   ¡¤  date: 2013/08/09
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ALIGN_HPP
#define LV_ALIGN_HPP

namespace lv
{
	template<class T>
	T	align_floor(T value, size_t alignment)
	{
		return value & ~static_cast<T>(alignment - 1);
	}

	template<class T>
	T	align_ceil(T value, size_t alignment)
	{
		return align_floor(value + static_cast<T>(alignment - 1), alignment);
	}

	template<class T>
	T *	align_floor(T * value, size_t alignment)
	{
		return reinterpret_cast<T*>(align_floor(reinterpret_cast<size_t>(value), alignment));
	}

	template<class T>
	T *	align_ceil(T * value, size_t alignment)
	{
		return reinterpret_cast<T*>(align_ceil(reinterpret_cast<size_t>(value), alignment));
	}

	//
	template<class T>
	bool	is_aligned(T value, size_t alignment)
	{
		return ((value & static_cast<T>(alignment - 1)) == 0);
	}

	template<class T>
	bool	is_aligned(T * value, size_t alignment)
	{
		return is_aligned(reinterpret_cast<size_t>(value), alignment);
	}
}

#endif