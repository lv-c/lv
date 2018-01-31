// *********************************************************************
//  Set   version:  1.0   ¡¤  date: 2013/08/20
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <set>
#include <algorithm>

namespace lv
{

#define STD_SET_OPERATION(op)	std::op (lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(ret, ret.begin()))

	// difference

	template<class T, class Pr, class Ax>
	std::set<T, Pr, Ax> & operator -= (std::set<T, Pr, Ax> & lhs, std::set<T, Pr, Ax> const & rhs)
	{
		std::set<T, Pr, Ax> ret;
		STD_SET_OPERATION(set_difference);

		lhs.swap(ret);
		return lhs;
	}

	template<class T, class Pr, class Ax>
	std::set<T, Pr, Ax> operator - (std::set<T, Pr, Ax> const & lhs, std::set<T, Pr, Ax> const & rhs)
	{
		std::set<T, Pr, Ax> ret;
		STD_SET_OPERATION(set_difference);

		return ret;
	}

	// join

	template<class T, class Pr, class Ax>
	std::set<T, Pr, Ax> & operator += (std::set<T, Pr, Ax> & lhs, std::set<T, Pr, Ax> const & rhs)
	{
		lhs.insert(rhs.begin(), rhs.end());
		return lhs;
	}

	template<class T, class Pr, class Ax>
	std::set<T, Pr, Ax> operator + (std::set<T, Pr, Ax> const & lhs, std::set<T, Pr, Ax> const & rhs)
	{
		std::set<T, Pr, Ax> ret = lhs;

		ret.insert(rhs.begin(), rhs.end());
		return ret;
	}

	// intersect

	template<class T, class Pr, class Ax>
	std::set<T, Pr, Ax> & operator &= (std::set<T, Pr, Ax> & lhs, std::set<T, Pr, Ax> const & rhs)
	{
		std::set<T, Pr, Ax> ret;
		STD_SET_OPERATION(set_intersection);

		lhs.swap(ret);
		return lhs;
	}

	template<class T, class Pr, class Ax>
	std::set<T, Pr, Ax> operator & (std::set<T, Pr, Ax> const & lhs, std::set<T, Pr, Ax> const & rhs)
	{
		std::set<T, Pr, Ax> ret;
		STD_SET_OPERATION(set_intersection);

		return ret;
	}

#undef STD_SET_OPERATION

}
