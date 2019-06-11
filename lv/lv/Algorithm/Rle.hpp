// *********************************************************************
//  Rle   version:  1.0   ��  date: 06/18/2008
//  --------------------------------------------------------------------
//		Rle �㷨��Ԫ�س��ȿ����� 1, 2, 4, 8
//	TODO : ʹ�� BufferRef �� ConstBufferRef ������ָ��
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

	// EleSize �����ǡ�1, 2, 4, 8.
	// dest_size ����������ڵ��� encode_bound(src_size), ������׳��쳣
	// src_size % EleSize ������ڡ�0
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
			// (������ͬ��ָ��ͬ��������n ���� (n - 1) * EleSize > 2 ��������ǲ���ͬ��)
			// ÿһ��Ϊ�ҵ���max_len ������ͬ�ģ����� m ����ͬ���Լ� n ����ͬ�� (m < max_len, n <= max_len)

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


				if ((same_num - 1) * EleSize > 2)	// A B B C  --  ��ʱ��B��B������һ��ȷ�һ��ѹ���ø���
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

			if (diff_num > 0)		// û����ͬ��
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

	

	// �û�Ӧ�ñ���ԭ���ݵĴ�С����dest�������㹻��
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
