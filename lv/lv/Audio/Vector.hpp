// *********************************************************************
//  Vector   version:  1.0   ¡¤  date: 10/24/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_AUDIO_VECTOR_HPP
#define LV_AUDIO_VECTOR_HPP


#include <boost/operators.hpp>
#include <boost/assert.hpp>

namespace lv
{
	template<typename T>
	class VectorT : boost::additive<VectorT<T>,
		boost::multipliable2<VectorT<T>, T,
		boost::modable2<VectorT<T>, T,
		boost::equality_comparable<VectorT<T> > > > >
	{
	public:

		enum {
			ele_num = 3
		};

		T	x;
		T	y;
		T	z;

		VectorT() : x(0), y(0), z(0) {}
		VectorT(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

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

		T const & operator[] (size_t i) const
		{
			BOOST_ASSERT(i < ele_num);
			return this->*mem_array[i];
		}

		T & operator[] (size_t i)
		{
			BOOST_ASSERT(i < ele_num);
			return this->*mem_array[i];
		}

		VectorT abs() const
		{
			return VectorT(std::abs(x), std::abs(y), std::abs(z));
		}

		// comparison
		friend bool operator == (PointT const& lhs, PointT const& rhs)
		{
			return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
		}

	private:

		static T PointT<T>::* const		mem_array[ele_num];

	};


	template<typename T>
	T VectorT<T>::* const mem_array[VectorT<T>::ele_num] = { &VectorT<T>::x, &VectorT<T>::y, &VectorT<T>::z};

}



#endif // LV_AUDIO_VECTOR_HPP