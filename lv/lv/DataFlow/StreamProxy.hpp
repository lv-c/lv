// *********************************************************************
//  StreamProxy   version:  1.0   ��  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_STREAMPROXY_HPP
#define LV_DATAFLOW_STREAMPROXY_HPP

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

namespace lv { namespace flow { namespace detail {

	template<class OArchive, class Src>
	class StreamProxyImpl
	{
		// "Class members are initialized in declaration order (regardless of
		// their order in the initialization list)."
		// So raw_os_ should be declared before oa_
		boost::iostreams::filtering_ostream raw_os_;

		OArchive oa_;

		BufferPtr	buffer_;

		Src	& source_;

	public:

		StreamProxyImpl(BufferPtr buf, Src & source)
			: buffer_(buf)
			, source_(source)
			, raw_os_(boost::iostreams::back_inserter(*buf))
			, oa_(raw_os_, boost::archive::no_header)
		{
		}

		~StreamProxyImpl()
		{
			raw_os_.flush();

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

		StreamProxy(BufferPtr buf, Src & source)
			: impl_(new impl_type(buf, source))
		{
		}

		template<typename T>
		StreamProxy & operator << (T const & t)
		{
			(*impl_) << t;

			return *this;
		}
	};

} } }


#endif