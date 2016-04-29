// *********************************************************************
//  StreamProxy   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_STREAMPROXY_HPP
#define LV_DATAFLOW_STREAMPROXY_HPP

#include <lv/StreamPtr.hpp>

namespace lv { namespace flow { namespace detail {

	template<class OArchive, class Src>
	class StreamProxyImpl
	{
		typedef typename OArchive	oarchive_type;
		typedef typename Src		source_type;

		OStreamPtr raw_os_;

		oarchive_type oa_;

		BufferPtr	buffer_;

		source_type	& source_;

	public:

		StreamProxyImpl(BufferPtr buf, OStreamPtr raw_os, source_type & source)
			: buffer_(buf)
			, raw_os_(raw_os)
			, source_(source)
			, oa_(*raw_os)
		{
		}

		~StreamProxyImpl()
		{
			raw_os_->flush();

			source_.push(buffer_);
		}

		template<typename T>
		void operator << (T const & t)
		{
			oa_ << t;
		}
	};

	template<class OArchive, class Src>
	class StreamProxy
	{
		typedef StreamProxyImpl<OArchive, Src> impl_type;
		std::auto_ptr<impl_type> impl_;

	public:

		StreamProxy(BufferPtr buf, OStreamPtr raw_os, Src & source)
			: impl_(new impl_type(buf, raw_os, source))
		{
		}

		// creates an empty StreamProxy
		StreamProxy()
		{
		}

		template<typename T>
		StreamProxy & operator << (T const & t)
		{
			if (impl_.get() != NULL)
			{
				(*impl_) << t;
			}

			return *this;
		}
	};

} } }


#endif