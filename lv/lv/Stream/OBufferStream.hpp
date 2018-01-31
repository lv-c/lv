// *********************************************************************
//  OBufferStream   version:  1.0   ¡¤  date: 04/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Buffer.hpp>
#include <lv/Stream/StreamBase.hpp>

namespace lv
{
	class OBufferStream : public StreamBase
	{
		Buffer &	buf_;

		BufferPtr	holder_;

	public:

		OBufferStream(Buffer & buf) 
			: buf_(buf) 
		{
		}


		OBufferStream(BufferPtr buf)
			: buf_(*buf)
			, holder_(buf)
		{
		}

		Buffer &	buffer()
		{
			return buf_;
		}

		BufferPtr	buffer_ptr()
		{
			BOOST_ASSERT(holder_);

			return holder_;
		}

		OBufferStream & put(char c)
		{
			buf_.push_back(c);

			return reset_state();
		}

		OBufferStream &	write(char const * data, streamsize size)
		{
			buffer::append(buf_, data, static_cast<size_t>(size));

			return reset_state();
		}

		/// there's no need to flush
		OBufferStream & flush()
		{
			return reset_state();
		}

		streampos	tellp() const
		{
			return static_cast<streamoff>(buf_.size());
		}

	private:

		OBufferStream &	reset_state()
		{
			setstate(std::ios_base::goodbit);
			return *this;
		}
	};
}