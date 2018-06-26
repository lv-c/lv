// *********************************************************************
//  Point   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#pragma once

#include <lv/Graphics/ContainerBase.hpp>


namespace lv
{
	template<class T>
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

		template<class U>
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

		template<class, class, size_t>	friend class ContainerBase;

		// array of pointers to member variables
		static T PointT::* const		mem_array[PointT::ele_num];
	};

	template<class T>
	T PointT<T>::* const PointT<T>::mem_array[PointT<T>::ele_num] = { &PointT<T>::x, &PointT<T>::y };

	
	using Point = PointT<int32_t>;
}