// *********************************************************************
//  IBufferStream   version:  1.0   ¡¤  date: 04/02/2010
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

	class IBufferStream : public StreamBase
	{
		ConstBufferRef	buf_;

		streampos		gpos_;

	public:

		IBufferStream(ConstBufferRef const & buf)
			: buf_(buf)
			, gpos_(0)
		{
		}

		IBufferStream &	read(char * data, streamsize size)
		{
			iostate	state = std::ios_base::goodbit;

			streamsize to_read = 0;
			if (size >= 0)
			{
				to_read = std::min<streamsize>(size, stream_size() - gpos_);
				std::copy(buf_.begin() + static_cast<size_t>(gpos_), buf_.begin() + static_cast<size_t>(gpos_ + to_read), data);
				
				gpos_ += to_read;
			}

			if (to_read != size)
			{
				state |= std::ios_base::failbit;
			}

			inner_setstate(state);
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

			switch (dir)
			{
			case std::ios_base::beg:
				break;
				
			case std::ios_base::cur:
				off += gpos_;
				break;

			case std::ios_base::end:
				off += stream_size();
				break;

			default:
				state = std::ios_base::failbit;
				break;
			}

			if (state == std::ios_base::goodbit)
			{
				if (off >= 0 && off <= stream_size())
				{
					gpos_ = off;
				}
				else
				{
					state = std::ios_base::failbit;
				}
			}

			inner_setstate(state);
			return *this;
		}

		IBufferStream &	ignore(streamsize n = 1, int delim = std::char_traits<char>::eof())
		{
			iostate	state = std::ios_base::goodbit;
			streampos pos = gpos_;
			streamsize size = stream_size();

			for (streamsize i = 0; i < n; ++i, pos += 1)
			{
				if (pos >= size)
				{
					break;
				}

				unsigned char c = buf_[static_cast<size_t>(pos)];
				if (static_cast<int>(c) == delim)
				{
					break;
				}
			}

			gpos_ = pos;

			inner_setstate(state);
			return *this;
		}

	private:

		streamsize	stream_size() const
		{
			return static_cast<streamsize>(buf_.size());
		}

		void	inner_setstate(iostate state)
		{
			if (gpos_ >= (streamsize) buf_.size())
			{
				state |= std::ios_base::eofbit;
			}

			setstate(state);
		}
	};

}