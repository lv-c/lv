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

#include <lv/BinaryStream/Serialize.hpp>
#include <lv/Buffer.hpp>

namespace lv { namespace bstream {


	template<>
	struct object_tag<Buffer> : copyable_buffer_tag {};			//

	template<typename T>
	struct object_tag<BufferRefT<T>, typename boost::enable_if<is_copyable<T> >::type> 
		: copyable_buffer_tag 
	{
	};

	template<typename T>
	struct object_tag<BufferRefT<T>, typename boost::disable_if<is_copyable<T> >::type> 
		: range_tag
	{
	};

	template<typename T>
	struct object_tag<ConstBufferRefT<T>, typename boost::enable_if<is_copyable<T> >::type> 
		: copyable_buffer_tag 
	{
	};

	template<typename T>
	struct object_tag<ConstBufferRefT<T>, typename boost::disable_if<is_copyable<T> >::type> 
		: range_tag
	{
	};

} }

#endif // LV_BINARYSTREAM_BUFFER_HPP