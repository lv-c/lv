// *********************************************************************
//  FourCC   version:  1.0   ¡¤  date: 10/26/2008
//  --------------------------------------------------------------------
//  "A FourCC (literally, four-character code) is a sequence of 
//	four bytes used to uniquely identify data formats." (wiki)
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FOURCC_HPP
#define LV_FOURCC_HPP

#include <lv/IntType.hpp>

namespace lv
{
	template<char c1, char c2, char c3, char c4>
	struct FourCC
	{
		typedef uint32	type;
		static type	const value = (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
	};
}


#endif