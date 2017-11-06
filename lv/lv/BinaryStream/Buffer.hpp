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

#include <lv/Buffer.hpp>
#include <lv/BinaryStream/Tags.hpp>


namespace lv::bstream
{
	template<>
	struct object_tag<Buffer> : primitive_buffer_tag {};			//

	template<typename T>
	struct object_tag<BufferRefT<T>, std::enable_if_t<is_primitive<T>::value> > 
		: primitive_buffer_tag 
	{
	};

	template<typename T>
	struct object_tag<BufferRefT<T>, std::enable_if_t<! is_primitive<T>::value> >
		: range_tag
	{
	};

	template<typename T>
	struct object_tag<ConstBufferRefT<T>, std::enable_if_t<is_primitive<T>::value> > 
		: primitive_buffer_tag 
	{
	};

	template<typename T>
	struct object_tag<ConstBufferRefT<T>, std::enable_if_t<! is_primitive<T>::value> >
		: range_tag
	{
	};

}

#endif