// *********************************************************************
//  Size   version:  1.0   ¡¤  date: 08/05/2008
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
	class SizeT : public ContainerBase<SizeT<T>, T, 2>,
		boost::additive<SizeT<T>, 
		boost::multiplicative2<SizeT<T>, T,
		boost::modable2<SizeT<T>, T> > >
	{
	public:

		T	cx;
		T	cy;

		// constructors 
		SizeT()
			: cx(0)
			, cy(0)
		{
		}
			
		explicit SizeT(T dim)
			: cx(dim)
			, cy(dim)
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

		SizeT abs() const
		{
			return SizeT(std::abs(cx), std::abs(cy));
		}

	private:

		template<class, class, size_t>	friend class ContainerBase;

		static T SizeT::* const mem_array[SizeT::ele_num];
	};

	template<class T>
	T SizeT<T>::* const SizeT<T>::mem_array[SizeT<T>::ele_num] = { &SizeT<T>::cx, &SizeT<T>::cy };

	using Size = SizeT<int32_t>;
}