// *********************************************************************
//  Buffer   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_BUFFER_HPP
#define LV_BINARYSTREAM_BUFFER_HPP

#include <lv/BinaryStream/Serializer.hpp>
#include <lv/Buffer.hpp>

namespace lv { namespace bstream {


	template<>
	struct object_tag<Buffer> : primitive_buffer_tag {};			//

	template<typename T>
	struct object_tag<BufferRefT<T>, typename std::enable_if<is_primitive<T>::value>::type> 
		: primitive_buffer_tag 
	{
	};

	template<typename T>
	struct object_tag<BufferRefT<T>, typename std::enable_if<! is_primitive<T>::value>::type>
		: range_tag
	{
	};

	template<typename T>
	struct object_tag<ConstBufferRefT<T>, typename std::enable_if<is_primitive<T>::value>::type> 
		: primitive_buffer_tag 
	{
	};

	template<typename T>
	struct object_tag<ConstBufferRefT<T>, typename std::enable_if<! is_primitive<T>::value>::type>
		: range_tag
	{
	};

} }

#endif