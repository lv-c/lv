// *********************************************************************
//  Size   version:  1.0   ¡¤  date: 08/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_SIZE_HPP
#define LV_SIZE_HPP

#include <boost/operators.hpp>
#include <boost/assert.hpp>

namespace lv
{
	template<typename T>
	class SizeT :boost::additive<SizeT<T>, 
		boost::multiplicative2<SizeT<T>, T,
		boost::modable2<SizeT<T>, T,
		boost::equality_comparable<SizeT<T> > > > >

	{
	public:

		enum {
			ele_num = 2
		};

		T	cx;
		T	cy;

		// constructors 
		SizeT()
			: cx(0)
			, cy(0)
		{
		}
		SizeT(T _cx, T _cy)
			: cx(_cx)
			, cy(_cy)
		{
		}
		// 
		void	set(T _cx, T _cy)
		{
			this->cx = _cx;
			this->cy = _cy;
		}
		// operators
		SizeT& operator += (SizeT const& sz)
		{
			cx += sz.cx;
			cy += sz.cy;
			return *this;
		}
		SizeT& operator -= (SizeT const& sz)
		{
			cx -= sz.cx;
			cy -= sz.cy;
			return *this;
		}
		SizeT& operator *= (T i)
		{
			cx *= i;
			cy *= i;
			return *this;
		}
		SizeT& operator /= (T i)
		{
			cx /= i;
			cy /= i;
			return *this;
		}
		SizeT& operator %= (T i)
		{
			cx %= i;
			cy %= i;
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


		SizeT abs() const
		{
			return SizeT(std::abs(cx), std::abs(cy));
		}

		// comparison
		friend bool operator == (SizeT const& lhs, SizeT const& rhs)
		{
			return lhs.cx == rhs.cx && lhs.cy == rhs.cy;
		}

	private:

		static T SizeT<T>::* const mem_array[ele_num];
	};

	typedef SizeT<int32>	Size;
}

#endif // LV_SIZE_HPP