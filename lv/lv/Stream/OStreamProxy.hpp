// *********************************************************************
//  OStreamProxy   version:  1.0   ¡¤  date: 04/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAM_OSTREAMPROXY_HPP
#define LV_STREAM_OSTREAMPROXY_HPP

#include <lv/Stream/StreamProxyBase.hpp>
#include <lv/Stream/OBufferStream.hpp>

#include <ostream>

namespace lv
{
	class OStreamProxy : public StreamProxyBase
	{
		std::ostream *	std_ios_;

		OBufferStream *	lv_ios_;

	public:

		OStreamProxy(std::ostream & ios)
			: StreamProxyBase(ios)
			, std_ios_(&ios)
			, lv_ios_(NULL)
		{
		}

		OStreamProxy(OBufferStream & ios)
			: StreamProxyBase(ios)
			, lv_ios_(&ios)
			, std_ios_(NULL)
		{
		}

		OStreamProxy & put(char c)
		{
			LV_STREAMPROXY_FW_CALL_1(put, c, (void));
			return *this;
		}

		OStreamProxy &	write(char const * data, streamsize size)
		{
			LV_STREAMPROXY_FW_CALL_2(write, data, size, (void));
			return *this;
		}

		OStreamProxy & flush()
		{
			LV_STREAMPROXY_FW_CALL_0(flush, (void));
			return *this;
		}

		std::streampos	tellp() const
		{
			LV_STREAMPROXY_FW_CALL_0(tellp, return);
		}

	};
}

#endif