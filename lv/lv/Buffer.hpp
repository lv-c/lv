// *********************************************************************
//  Buffer   version:  1.0   ·  date: 11/18/2007
//  --------------------------------------------------------------------
//		用 vector<char> 作为 Buffer 类
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BUFFER_HPP
#define LV_BUFFER_HPP

#include <vector>
#include <boost/shared_ptr.hpp>

namespace lv
{
	typedef std::vector<char>	Buffer;
	typedef boost::shared_ptr<Buffer>	BufferPtr;

	namespace buffer
	{
		// Write to the end of the buffer . You'd better reserve enough space to prevent frequent
		// memory allocation.
		inline void write(BufferPtr buf, void const* data, size_t size)
		{
			buf->insert(buf->end(), static_cast<char const*>(data), static_cast<char const*>(data) + size);
		}
	}
}

#endif // LV_BUFFER_HPP
