// *********************************************************************
//  PixelFormat   version:  1.0   ·  date: 09/06/2008
//  --------------------------------------------------------------------
//  纹理的像素格式 以及和 boost::gil 中数据类型的映射。
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/gil/typedefs.hpp>
#include <boost/gil/image.hpp>

#include <type_traits>


namespace lv
{

	enum PixelFormat
	{
		PF_RGB,		// 24 bits
		PF_ARGB,	// 32 bits
		PF_L8		// 8 bits luminace
	};

	template<int PF>
	struct PixelBytes;

	template<>
	struct PixelBytes<PF_RGB> : std::integral_constant<size_t, 3>
	{
	};
	
	template<>
	struct PixelBytes<PF_ARGB> : std::integral_constant<size_t, 4>
	{
	};
	
	template<>
	struct PixelBytes<PF_L8> : std::integral_constant<size_t, 1>
	{
	};


	// mappings to boost::gil types. PF_RGB is not yet supported.
	namespace GIL
	{

		// pixel type

		template<int PF>
		struct PixelT;

		template<>
		struct PixelT<PF_ARGB>
		{
			using type = boost::gil::argb32_pixel_t;
		};

		template<>
		struct PixelT<PF_L8>
		{
			using type = boost::gil::gray8_pixel_t;
		};

		// x iterator type

		template<int PF, bool IsMutable = true>
		struct XIteratorT 
			: boost::gil::iterator_type_from_pixel<typename PixelT<PF>::type, false, false, IsMutable>
		{
		};

		// view type
		template<int PF, bool IsMutable = true>
		struct ViewT
		{
			using type = typename boost::gil::type_from_x_iterator<typename XIteratorT<PF, IsMutable>::type>::view_t;
		};
	}

}

