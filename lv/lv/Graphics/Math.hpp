// *********************************************************************
//  Math   version:  1.0   ��  date: 08/26/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Config.hpp>
#include <lv/Graphics/Point.hpp>
#include <lv/Graphics/Vector.hpp>

#include <cmath>
#include <numeric>
#include <type_traits>


namespace lv::math
{
	float const PI		=	3.14159265f;
	float const PId2	=	1.57079632f;
	float const PId4	=	0.78539816f;


	template<class T, class = void>
	struct FloatType 
		: boost::mpl::identity<float> 
	{
	};

	template<class T>
	struct FloatType<T, std::enable_if_t<std::is_floating_point_v<T> > >
		: boost::mpl::identity<T>
	{
	};

	template<class T>
	typename FloatType<T>::type	sqrt(T v)
	{
		return std::sqrt(static_cast<typename FloatType<T>::type>(v));
	}


	template<class T>
	typename FloatType<T>::type	dist(PointT<T> const & lhs, PointT<T> const & rhs)
	{
		return math::sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
	}

#ifdef LV_WIN32

	inline __declspec(naked) float	rsqrt(float v)
	{
		static_assert(sizeof(float) == 4);

		__asm{
			rsqrtss xmm0, dword ptr [esp + 4]
			movss [esp + 4], xmm0
			fld dword ptr [esp + 4]
			retn
		}
	}

#endif

	template<class T>
	T	dot(VectorT<T> const & lhs, VectorT<T> const & rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
	}

	template<class T>
	VectorT<T>	cross(VectorT<T> const & lhs, VectorT<T> const & rhs)
	{
		return VectorT<T>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}

	template<class T>
	T	length_sq(VectorT<T> const & v)
	{
		return dot(v, v);
	}

	template<class T>
	typename FloatType<T>::type	length(VectorT<T> const & v)
	{
		return math::sqrt(length_sq(v));
	}

#ifdef LV_WIN32

	inline Vector3f	normalize(Vector3f const & v)
	{
		return v * rsqrt(length_sq(v));
	}

#endif


	template<class T>
	bool	intersect(VectorT<T> const & v0, VectorT<T> const & v1, VectorT<T> const & v2,
		VectorT<T> const & orig, VectorT<T> const & dir, VectorT<T> & ret)
	{
		// find vectors for ( two edges sharing vert0
		VectorT<T> edge1 = v1 - v0;
		VectorT<T> edge2 = v2 - v0;

		// begin calculating determinant - also used to calculate U parameter
		VectorT<T> pvec = cross(dir, edge2);

		// if determinant is near zero, ray lies in plane of triangle
		T det = dot(edge1, pvec);
		if (det <= std::numeric_limits<T>::epsilon())
		{
			return false;
		}

		// calculate distance from vert0 to ray origin
		VectorT<T> tvec = orig - v0;

		//calculate U parameter and test bounds
		T u = dot(tvec, pvec);
		if (u < T(0) || u > det)
		{
			return false;
		}

		// prepare to test V parameter
		VectorT<T> qvec = cross(tvec, edge1);

		// calculate V parameter and test bounds
		T v = dot(dir, qvec);
		if (v < T(0) || u + v > det)
		{
			return false;
		}

		T t = dot(edge2, qvec) / det;
		ret = orig + dir * t;

		return true;
	}


	/*
	template<class T>
	bool	intersect(TriangleT<VectorT<T> > const & tri, VectorT<T> const & orig, VectorT<T> const & dir, 
		VectorT<T> & ret)
	{
		return intersect(tri.v0, tri.v1, tri.v2, orig, dir, ret);
	}
	*/

}
