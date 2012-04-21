// *********************************************************************
//  IStreamProxy   version:  1.0   ¡¤  date: 04/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAM_ISTREAMPROXY_HPP
#define LV_STREAM_ISTREAMPROXY_HPP

#include <lv/Stream/StreamProxyBase.hpp>
#include <lv/Stream/IBufferStream.hpp>

#include <istream>

namespace lv
{
	class IStreamProxy : public StreamProxyBase
	{
		std::istream *	std_ios_;

		IBufferStream *	lv_ios_;

	protected:

		// delayed initialization

		IStreamProxy()
			: std_ios_(NULL)
			, lv_ios_(NULL)
		{
		}

		void	set(std::istream * std_ios, IBufferStream * lv_ios)
		{
			StreamProxyBase::set(std_ios, lv_ios);

			this->std_ios_ = std_ios;
			this->lv_ios_ = lv_ios;
		}

	public:

		IStreamProxy(std::istream & ios)
			: StreamProxyBase(ios)
			, std_ios_(&ios)
			, lv_ios_(NULL)
		{
		}

		IStreamProxy(IBufferStream & ios)
			: StreamProxyBase(ios)
			, lv_ios_(&ios)
			, std_ios_(NULL)
		{
		}

		IStreamProxy &	read(char * data, streamsize size)
		{
			LV_STREAMPROXY_FW_CALL_2(read, data, size, (void));
			return *this;
		}

		std::streampos	tellg() const
		{
			LV_STREAMPROXY_FW_CALL_0(tellg, return);
		}

		IStreamProxy &	seekg(streampos pos)
		{
			LV_STREAMPROXY_FW_CALL_1(seekg, pos, (void));
			return *this;
		}

		IStreamProxy &	seekg(streamoff off, std::ios_base::seekdir dir)
		{
			LV_STREAMPROXY_FW_CALL_2(seekg, off, dir, (void));
			return *this;
		}

		IStreamProxy &	ignore(streamsize n = 1, int delim = std::char_traits<char>::eof())
		{
			LV_STREAMPROXY_FW_CALL_2(ignore, n, delim, (void));
			return *this;
		}
	};
}

#endif