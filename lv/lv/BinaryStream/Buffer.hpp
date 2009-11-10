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

	template<>
	struct object_tag<BufferRef> : copyable_buffer_tag {};

	template<>
	struct object_tag<ConstBufferRef> : copyable_buffer_tag{};

} }

#endif // LV_BINARYSTREAM_BUFFER_HPP