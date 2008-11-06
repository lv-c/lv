// *********************************************************************
//  Rect   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_RECT_HPP
#define LV_RECT_HPP

#include <lv/Graphics/Point.hpp>
#include <lv/Graphics/Size.hpp>

namespace lv
{
	template<typename T>
	class RectT : boost::additive2<RectT<T>, PointT<T>,
		boost::multiplicative2<RectT<T>, T,
		boost::equality_comparable<RectT<T> > > > 
	{
	public:

		enum {
			ele_num = 4
		};

		T	left;
		T	top;
		T	right;
		T	bottom;



		// constructors
		RectT()
			: left(0)
			, top(0)
			, right(0)
			, bottom(0)
		{
		}
		RectT(T _left, T _top, T _right, T _bottom)
			: left(_left)
			, top(_top)
			, right(_right)
			, bottom(_bottom)
		{
		}
		RectT(PointT<T> const& top_left, PointT<T> const& right_bottom)
		{
			set(top_left.x, top_left.y, right_bottom.x, right_bottom.y);
		}
		RectT(PointT<T> const& top_left, SizeT<T> const & sz)
		{
			set(top_left.x, top_left.y, top_left.x + sz.cx, top_left.y + sz.cy);
		}

		//
		void set(T _left, T _top, T _right, T _bottom)
		{
			this->left = _left;
			this->top = _top;
			this->right = _right;
			this->bottom = _bottom;
		}
		void move_to(T x, T y)
		{
			right += x - left;
			bottom += y - top;
			left = x;
			top = y;
		}
		void resize(T width, T height)
		{
			right = left + width;
			bottom = top + height;
		}
		// top-left corner
		PointT<T>	pos() const
		{
			return PointT<T>(left, top);
		}
		SizeT<T> size() const
		{
			return SizeT<T>(width(), height());
		}
		T width() const
		{
			return right - left;
		}
		T height() const
		{
			return bottom - top;
		}
		PointT<T>	center() const
		{
			return PointT<T>((left + right) / 2, (top + bottom) / 2);
		}

		PointT<T>	top_left() const
		{
			return Point<T>(left, top);
		}
		PointT<T>	top_right() const
		{
			return Point<T>(right, top);
		}
		PointT<T>	bottom_left() const
		{
			return Point<T>(left, bottom);
		}
		PointT<T>	bottom_right() const
		{
			return Point<T>(right, bottom);
		}

		//
		bool contains(PointT<T> const& pt)
		{
			return pt.x >= left && pt.x < right && pt.y >= top && pt.y < bottom;
		}
		bool contains(RectT const& rect)
		{
			return rect.left >= left && rect.right <= right && rect.top >= top && rect.bottom <= bottom;
		}

		// operators
		RectT& operator += (PointT<T> const& pt)
		{
			left += pt.x;
			right += pt.x;
			top += pt.y;
			bottom += pt.y;
			return *this;
		}
		RectT& operator -= (PointT<T> const& pt)
		{
			left -= pt.x;
			right -= pt.x;
			top -= pt.y;
			bottom -= pt.y;
			return *this;
		}
		RectT& operator *= (T i)
		{
			left *= i;
			right *= i;
			top *= i;
			bottom *= i;
			return *this;
		}
		RectT& operator /= (T i)
		{
			left /= i;
			right /= i;
			top /= i;
			bottom /= i;
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

		// comparison
		friend bool operator == (RectT const& lhs, RectT const& rhs)
		{
			for(size_t i = 0; i < ele_num; ++i)
			{
				if(lhs[i] != rhs[i])
					return false;
			}
			return true;
		}

	private:

		static T RectT<T>::* const mem_array[ele_num];
	};

	template<typename T>
	T RectT<T>::* const mem_array[RectT<T>::ele_num] = {
		&RectT<T>::left, &RectT<T>::top, &RectT<T>::right, &RectT<T>::bottom
	};

	typedef RectT<int32>	Rect;
}


#endif // LV_RECT_HPP