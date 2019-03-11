// *********************************************************************
//  String   version:  1.0   ¡¤  date: 2018/08/17
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2018 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Operations.hpp>
#include <lv/ContainerAdaptor/String.hpp>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/algorithm/string/trim.hpp>


namespace lv::algo
{
	template<class RangeT, class SequenceT>
	void	to_base64(RangeT const & src, SequenceT & dest)
	{
		using namespace boost::archive::iterators;
		using base64 = base64_from_binary<transform_width<
			typename boost::range_const_iterator<RangeT>::type, 6, 8> >;

		lv::assign(dest, base64(std::begin(src)), base64(std::end(src)));

		constexpr size_t block_num = 3;
		size_t pad_num = (block_num - std::size(src) % block_num) % block_num;

		for (size_t i = 0; i < pad_num; ++i)
		{
			lv::push(dest, '=');
		}
	}

	template<class SequenceT>
	SequenceT	to_base64(SequenceT const & src)
	{
		SequenceT ret;
		to_base64(src, ret);
		return ret;
	}



	template<class RangeT, class SequenceT>
	void	from_base64(RangeT const & src, SequenceT & dest)
	{
		using namespace boost::archive::iterators;
		using base64 = transform_width<binary_from_base64<
			typename boost::range_const_iterator<RangeT>::type>, 8, 6>;

		lv::assign(dest, base64(std::begin(src)), base64(std::end(src)));
		boost::algorithm::trim_right_if(dest, [](auto v) { return v == '\0'; });
	}

	template<class SequenceT>
	SequenceT	from_base64(SequenceT const & src)
	{
		SequenceT ret;
		from_base64(src, ret);
		return ret;
	}


	// the simplest encryption
	template<class Range1T, class Range2T>
	void	mix(Range1T & data, Range2T const & code)
	{
		auto it = std::begin(code);
		uint8_t index = 0;

		for (auto & v : data)
		{
			v = (~v) ^ (*it++) ^ (index++);

			if (it == std::end(code))
			{
				it = std::begin(code);
			}
		}
	}
}
