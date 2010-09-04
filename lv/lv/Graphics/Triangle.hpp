// *********************************************************************
//  Triangle   version:  1.0   ¡¤  date: 09/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GRAPHICS_TRIANGLE_HPP
#define LV_GRAPHICS_TRIANGLE_HPP

#include <lv/Graphics/ContainerBase.hpp>

namespace lv
{
	template<typename T>
	class TriangleT : public ContainerBase<TriangleT<T>, T, 3>
	{
	public:

		TriangleT() {}

		TriangleT(T const & _v0, T const & _v1, T const & _v2)
			: v0(_v0)
			, v1(_v1)
			, v2(_v2)
		{
		}

		T	v0;
		T	v1;
		T	v2;

		void	set(T const & _v0, T const & _v1, T const & _v2)
		{
			this->v0 = _v0;
			this->v1 = _v1;
			this->v2 = _v2;
		}

		TriangleT & operator *= (T i)
		{
			v0 *= i;
			v1 *= i;
			v2 *= i;
			return *this;
		}

		TriangleT & operator /= (T i)
		{
			v0 /= i;
			v1 /= i;
			v2 /= i;
			return *this;
		}

	private:

		static T TriangleT::* const		mem_array[ele_num];

	};

	template<typename T>
	T TriangleT<T>::* const TriangleT<T>::mem_array[TriangleT<T>::ele_num] = { &TriangleT<T>::v0, &TriangleT<T>::v1, &TriangleT<T>::v2};
}

#endif