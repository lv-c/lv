// *********************************************************************
//  ContainerBase   version:  1.0   ¡¤  date: 09/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/operators.hpp>

namespace lv
{
	template<class Derived, class T, size_t N>
	class ContainerBase : boost::equality_comparable<Derived>
	{
	public:

		enum {
			ele_num	=	N
		};

		using value_type = T;
		using reference = T &;
		using const_reference = T const &;
		using iterator = T *;
		using const_iterator = T const *;

		size_t		size() const
		{
			return ele_num;
		}

		iterator	begin()
		{
			return &(static_cast<Derived*>(this)->*Derived::mem_array[0]);
		}

		const_iterator	begin() const
		{
			return &(static_cast<Derived const*>(this)->*Derived::mem_array[0]);
		}

		iterator	end()
		{
			return begin() + ele_num;
		}

		const_iterator	end() const
		{
			return begin() + ele_num;
		}

		reference operator[] (size_t i)
		{
			BOOST_ASSERT(i < ele_num);
			return *(begin() + i);
		}

		const_reference operator[] (size_t i) const
		{
			BOOST_ASSERT(i < ele_num);
			return *(begin() + i);
		}


		// comparison
		friend bool operator == (Derived const & lhs, Derived const & rhs)
		{
			return std::equal(lhs.begin(), lhs.end(), rhs.begin());
		}

	};
}
