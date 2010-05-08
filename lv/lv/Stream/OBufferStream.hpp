// *********************************************************************
//  OBufferStream   version:  1.0   ��  date: 04/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAM_OBUFFERSTREAM_HPP
#define LV_STREAM_OBUFFERSTREAM_HPP

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

		OBufferStream & put(char c)
		{
			buf_.push_back(c);

			return reset_state();
		}

		OBufferStream &	write(char const * data, streamsize size)
		{
			buffer::write(buf_, data, size);

			return reset_state();
		}

		/// there's no need to flush
		OBufferStream & flush()
		{
			return reset_state();
		}

		streampos	tellp() const
		{
			return buf_.size();
		}

	private:

		OBufferStream &	reset_state()
		{
			setstate(std::ios_base::goodbit);
			return *this;
		}
	};
}

#endif