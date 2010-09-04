// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 10/24/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_GRAPHICS_VECTOR_HPP
#define LV_GRAPHICS_VECTOR_HPP

#include <lv/Graphics/ContainerBase.hpp>

namespace lv
{
	template<typename T>
	class VectorT : public ContainerBase<VectorT<T>, T, 3>,
		boost::additive<VectorT<T>,
		boost::multiplicative2<VectorT<T>, T,
		boost::modable2<VectorT<T>, T> > >
	{
	public:

		T	x;
		T	y;
		T	z;

		VectorT() : x(0), y(0), z(0) {}
		VectorT(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

		template<typename U>
		explicit VectorT(VectorT<U> const & vec)
		{
			x = static_cast<T>(vec.x);
			y = static_cast<T>(vec.y);
			z = static_cast<T>(vec.z);
		}

		void	set(T _x, T _y, T _z)
		{
			this->x = _x;
			this->y = _y;
			this->z = _z;
		}

		VectorT & operator += (VectorT const & vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}
		VectorT & operator -= (VectorT const & vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}
		VectorT & operator *= (T i)
		{
			x *= i;
			y *= i;
			z *= i;
			return *this;
		}
		VectorT & operator /= (T i)
		{
			x /= i;
			y /= i;
			z /= i;
			return *this;
		}
		VectorT & operator %= (T i)
		{
			x %= i;
			y %= i;
			z %= i;
			return *this;
		}
		

		VectorT abs() const
		{
			return VectorT(std::abs(x), std::abs(y), std::abs(z));
		}

	private:

		template<class, typename, size_t>	friend class ContainerBase;

		static T VectorT::* const		mem_array[ele_num];

	};


	template<typename T>
	T VectorT<T>::* const VectorT<T>::mem_array[VectorT<T>::ele_num] = { &VectorT<T>::x, &VectorT<T>::y, &VectorT<T>::z};

	typedef VectorT<float>	Vector3f;

}

#endif