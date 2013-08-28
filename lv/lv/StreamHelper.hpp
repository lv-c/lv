// *********************************************************************
//  StreamHelper   version:  1.0   ¡¤  date: 2013/08/23
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAMHELPER_HPP
#define LV_STREAMHELPER_HPP

#include <lv/IntType.hpp>

#include <ostream>

namespace lv
{

	// TODO : supports wostream

	class write_tabs
	{
		size_t	count_;

	public:

		explicit write_tabs(size_t count)
			: count_(count)
		{
		}

		friend std::ostream & operator << (std::ostream & os, write_tabs tabs)
		{
			for(size_t i = 0; i < tabs.count_; ++i)
			{
				os << '\t';
			}

			return os;
		}
	};

	namespace detail
	{
		template<typename T>
		class hex_impl
		{
			T	t_;

		public:

			hex_impl(T t) : t_(t) {}

			friend std::ostream & operator << (std::ostream & os, hex_impl const & h)
			{
				return os << "0x" << std::hex << lv::widen_int(h.t_) << std::dec;
			}
		};
	}


	template<typename T>
	detail::hex_impl<T> hex(T t)
	{
		return detail::hex_impl<T>(t);
	}
}

#endif