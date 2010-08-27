// *********************************************************************
//  Math   version:  1.0   ¡¤  date: 08/26/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GRAPHICS_MATH_HPP
#define LV_GRAPHICS_MATH_HPP

#include <lv/Graphics/Vector.hpp>

#include <cmath>

namespace lv { namespace math {


	float const PI		=	3.14159265f;
	float const PId2	=	1.57079632f;
	float const PId4	=	0.78539816f;

	inline __declspec(naked) float	rsqrt(float v)
	{
		__asm{
			rsqrtss xmm0, dword ptr [esp + 4]
			movss [esp + 4], xmm0
			fld dword ptr [esp + 4]
			retn
		}
	}

	template<typename T>
	T	dot(VectorT<T> const & lhs, VectorT<T> const & rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
	}

	template<typename T>
	T	length_sq(VectorT<T> const & v)
	{
		return dot(v, v);
	}

	template<typename T>
	T	length(VectorT<T> const & v)
	{
		return std::sqrt(length_sq(v));
	}

	inline Vector3f	normalize(Vector3f const & v)
	{
		return v * rsqrt(length_sq(v));
	}



} }

#endif