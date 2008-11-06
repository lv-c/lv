// *********************************************************************
//  ParamType   version:  1.0   ��  date: 10/28/2008
//  --------------------------------------------------------------------
//  ������������
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_PARAMTYPE_HPP
#define LV_PARAMTYPE_HPP

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>

namespace lv
{
	template<typename T, class Enabled = void>
	struct ParamType
	{
		typedef T const &	type;
	};

	template<typename T>
	struct ParamType<T, boost::enable_if<boost::is_arithmetic<T> > >
	{
		typedef T	type;
	};
}

#endif // LV_PARAMTYPE_HPP