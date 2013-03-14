// *********************************************************************
//  Point   version:  1.0   ��  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_POINT_HPP
#define LV_POINT_HPP

#include <lv/Graphics/ContainerBase.hpp>
#include <lv/IntType.hpp>

namespace lv
{
	template<typename T>
	class PointT : public ContainerBase<PointT<T>, T, 2>,
		boost::additive<PointT<T>, 
		boost::multiplicative2<PointT<T>, T,
		boost::modable2<PointT<T>, T> > >

	{
	public:

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

		void	offset(T _x, T _y)
		{
			this->x += _x;
			this->y += _y;
		}

		PointT abs() const
		{
			return PointT(std::abs(x), std::abs(y));
		}

	private:

		template<class, typename, size_t>	friend class ContainerBase;

		// array of pointers to member variables
		static T PointT::* const		mem_array[ele_num];
	};

	template<typename T>
	T PointT<T>::* const PointT<T>::mem_array[PointT<T>::ele_num] = { &PointT<T>::x, &PointT<T>::y };

	
	typedef PointT<int32>	Point;
}

#endif