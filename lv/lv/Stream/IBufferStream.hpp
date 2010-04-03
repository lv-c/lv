// *********************************************************************
//  IBufferStream   version:  1.0   ¡¤  date: 04/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAM_IBUFFERSTREAM_HPP
#define LV_STREAM_IBUFFERSTREAM_HPP

#include <lv/Buffer.hpp>
#include <lv/Stream/StreamBase.hpp>

namespace lv
{

	class IBufferStream : public StreamBase
	{
		ConstBufferRef	buf_;

		streampos		gpos_;

	public:

		IBufferStream(ConstBufferRef buf)
			: buf_(buf)
			, gpos_(0)
		{
		}

		IBufferStream &	read(char * data, streamsize size)
		{
			iostate	state = std::ios_base::goodbit;

			streamsize to_read = 0;
			if(size >= 0)
			{
				to_read = std::min<streamsize>(size, buf_.size() - gpos_);
				std::copy(buf_.begin() + gpos_, buf_.begin() + gpos_ + to_read, data);
				
				gpos_ += to_read;
			}

			if(to_read != size)
			{
				state |= std::ios_base::eofbit | std::ios_base::failbit;
			}

			setstate(state);
			return *this;
		}

		streampos	tellg() const
		{
			return gpos_;
		}

		IBufferStream &	seekg(streampos pos)
		{
			return seekg(pos, std::ios_base::beg);
		}

		IBufferStream & seekg(streamoff off, std::ios_base::seekdir dir)
		{
			iostate	state = std::ios_base::goodbit;

			switch(dir)
			{
			case std::ios_base::beg:
				break;
				
			case std::ios_base::cur:
				off += gpos_;
				break;

			case std::ios_base::end:
				off += buf_.size();
				break;

			default:
				state = std::ios_base::failbit;
				break;
			}

			if(state == std::ios_base::goodbit)
			{
				if(off >= 0 && off <= static_cast<streamsize>(buf_.size()))
				{
					gpos_ = off;
				}
				else
				{
					state = std::ios_base::failbit;
				}
			}

			setstate(state);
			return *this;
		}

		IBufferStream &	ignore(streamsize n = 1, int delim = std::char_traits<char>::eof())
		{
			iostate	state = std::ios_base::goodbit;
			streampos pos = gpos_;
			streamsize size = buf_.size();

			for(streamsize i = 0; i < n; ++i, pos += 1)
			{
				if(pos >= size)
				{
					state = std::ios_base::eofbit;
					break;
				}

				if(buf_[pos] == delim)
				{
					break;
				}
			}

			setstate(state);
			return *this;
		}
	};

}

#endif