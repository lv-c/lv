// *********************************************************************
//  PixelFormat   version:  1.0   ·  date: 09/06/2008
//  --------------------------------------------------------------------
//  纹理的像素格式 以及和 boost::gil 中数据类型的映射。
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_PIXELFORMAT_HPP
#define LV_PIXELFORMAT_HPP

#include <boost/gil/typedefs.hpp>
#include <boost/gil/image.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/size_t.hpp>

namespace lv
{

	enum PixelFormat
	{
		PF_RGB,		// 24 bits
		PF_ARGB,	// 32 bits
		PF_L8		// 8 bits luminace
	};

	template <int PF>
	struct PixelBytes;

	template <>
	struct PixelBytes<PF_RGB> : boost::mpl::size_t<3>
	{
	};
	
	template <>
	struct PixelBytes<PF_ARGB> : boost::mpl::size_t<4>
	{
	};
	
	template <>
	struct PixelBytes<PF_L8> : boost::mpl::size_t<1>
	{
	};


	// mappings to boost::gil types. PF_RGB is not yet supported.
	namespace GIL
	{

		// pixel type

		template <int PF>
		struct PixelT;

		template <>
		struct PixelT<PF_ARGB>
		{
			typedef boost::gil::argb32_pixel_t	type;
		};

		template <>
		struct PixelT<PF_L8>
		{
			typedef boost::gil::gray8_pixel_t	type;
		};

		// x iterator type

		template <int PF, bool IsMutable = true>
		struct XIteratorT 
			: boost::gil::iterator_type_from_pixel<typename PixelT<PF>::type, false, false, IsMutable>
		{
		};

		// view type
		template <int PF, bool IsMutable = true>
		struct ViewT
		{
			typedef typename boost::gil::type_from_x_iterator<typename XIteratorT<PF, IsMutable>::type>::view_t type;
		};
	}

}



#endif // LV_PIXELFORMAT_HPP