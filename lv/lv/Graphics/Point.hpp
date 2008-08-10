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

namespace lv
{
	template<typename T>
	struct PointT : boost::additive<PointT<T>, 
		boost::multiplicative2<PointT<T>, T,
		boost::modable2<PointT<T>, T,
		boost::equality_comparable<PointT<T> > > > >

	{
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

		PointT abs() const
		{
			return PointT(std::abs(x), std::abs(y));
		}

		// comparison
		friend bool operator == (PointT const& lhs, PointT const& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}
	};


	typedef PointT<int32>	Point;

}

#endif // LV_POINT_HPP