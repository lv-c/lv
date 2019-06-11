// *********************************************************************
//  Rle   version:  1.0   ·  date: 06/18/2008
//  --------------------------------------------------------------------
//		Rle 算法。元素长度可以是 1, 2, 4, 8
//	TODO : 使用 BufferRef 及 ConstBufferRef 来代替指针
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/IntType.hpp>
#include <lv/Ensure.hpp>

#include <boost/assert.hpp>


namespace lv::rle
{
	inline size_t encode_bound(size_t src_len)
	{
		return src_len + (src_len >> 6) + 1;
	}

	// EleSize 可以是　1, 2, 4, 8.
	// dest_size 必须满足大于等于 encode_bound(src_size), 否则会抛出异常
	// src_size % EleSize 必须等于　0
	template<size_t EleSize>
	size_t encode(char const * src, size_t src_size, char * dest, size_t dest_size)
	{
		BOOST_ASSERT(src != dest && dest_size >= encode_bound(src_size));
		BOOST_ASSERT(src_size % EleSize == 0);

		LV_ENSURE(dest_size >= encode_bound(src_size), std::invalid_argument("the destination buffer is too small"));

		using element = IntType<EleSize>::type;
		size_t ele_num = src_size / EleSize;

		element const * src_ele = reinterpret_cast<element const*>(src);
		char * dest_pt = dest;

		size_t const max_len = 127;

		size_t cur = 0;
		while (cur < ele_num)
		{
			// (以下相同的指相同的数量　n 满足 (n - 1) * EleSize > 2 ，否则就是不相同的)
			// 每一轮为找到　max_len 个不相同的，或者 m 个不同的以及 n 个相同的 (m < max_len, n <= max_len)

			size_t diff_num = 0;
			size_t round_start = cur;

			while (diff_num < max_len && cur < ele_num)
			{
				size_t same_start = cur;
				size_t same_num = 1;


				while (cur < ele_num - 1 && src_ele[cur + 1] == src_ele[cur] && same_num < max_len)
				{
					same_num ++;
					cur ++;
				}
				cur ++;


				if ((same_num - 1) * EleSize > 2)	// A B B C  --  此时　B　B　不放一起比放一起压缩得更好
				{
					if (diff_num > 0)
					{
						*dest_pt++ = diff_num;
						memcpy(dest_pt, &src_ele[round_start], diff_num * EleSize);
						dest_pt += diff_num * EleSize;
					}

					diff_num = 0;


					*dest_pt ++ = 0x80 | same_num;
					*reinterpret_cast<element*>(dest_pt) = src_ele[same_start];
					dest_pt += EleSize;

					break;
				}
				else
					diff_num += same_num;
			}

			if (diff_num > 0)		// 没有相同的
			{
				if (diff_num > max_len)
				{
					cur -= diff_num - max_len;
					diff_num = max_len;
				}

				*dest_pt++ = diff_num;
				memcpy(dest_pt, &src_ele[round_start], diff_num * EleSize);
				dest_pt += diff_num * EleSize;
			
				diff_num = 0;
			}
		}

		return dest_pt - dest;;
	}

	

	// 用户应该保存原数据的大小，　dest　必须足够大
	template<size_t EleSize>
	size_t decode(char const * src, size_t src_size, char * dest)
	{
		using element = IntType<EleSize>::type;

		element * dest_ele = reinterpret_cast<element*>(dest);
		size_t index = 0;
		
		while (index < src_size)
		{
			unsigned char flag = src[index];
			index ++;
			if (flag & 0x80)		// sequence of the same elements
			{
				size_t num = flag & 0x7F;
				std::fill(dest_ele, dest_ele + num, *reinterpret_cast<element const*>(&src[index]));
				
				index += EleSize;
				dest_ele += num;
			}
			else
			{
				size_t num = flag;
				memcpy(dest_ele, &src[index], num * EleSize);

				index += num * EleSize;
				dest_ele += num;
			}
		}
		
		BOOST_ASSERT(index == src_size);

		return reinterpret_cast<char*>(dest_ele) - dest;
	}

}
