// *********************************************************************
//  Point   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_POINT_HPP
#define LV_POINT_HPP

#include <boost/operators.hpp>
#include <boost/assert.hpp>

namespace lv
{
	template<typename T>
	class PointT : boost::additive<PointT<T>, 
		boost::multiplicative2<PointT<T>, T,
		boost::modable2<PointT<T>, T,
		boost::equality_comparable<PointT<T> > > > >

	{
	public:

		enum {
			ele_num = 2
		};


		T	x;
		T	y;

		// constructors 
		PointT()
			: x(0)
			, y(0)
		{
		}
		PointT(T _x, T _y)
			: x(_x)
			, y(_y)
		{
		}

		template<typename U>
		PointT(PointT<U> const & pt)
			: x(pt.x)
			, y(pt.y)
		{
		}

		// 
		void	set(T _x, T _y)
		{
			this->x = _x;
			this->y = _y;
		}
		// operators
		PointT& operator += (PointT const& pt)
		{
			x += pt.x;
			y += pt.y;
			return *this;
		}
		PointT& operator -= (PointT const& pt)
		{
			x -= pt.x;
			y -= pt.y;
			return *this;
		}
		PointT& operator *= (T i)
		{
			x *= i;
			y *= i;
			return *this;
		}
		PointT& operator /= (T i)
		{
			x /= i;
			y /= i;
			return *this;
		}
		PointT& operator %= (T i)
		{
			x %= i;
			y %= i;
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


		void	offset(T _x, T _y)
		{
			this->x += _x;
			this->y += _y;
		}

		PointT abs() const
		{
			return PointT(std::abs(x), std::abs(y));
		}

		// comparison
		friend bool operator == (PointT const& lhs, PointT const& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}

	private:

		// array of pointers to member variables
		static T PointT<T>::* const		mem_array[ele_num];
	};


	typedef PointT<int32>	Point;

}

#endif // LV_POINT_HPP