// *********************************************************************
//  IStreamFactory   version:  1.0   ��  date: 04/14/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAM_ISTREAMFACTORY_HPP
#define LV_STREAM_ISTREAMFACTORY_HPP

#include <lv/Stream/StreamFactory.hpp>
#include <lv/StreamPtr.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

namespace lv
{
	class IStreamFactory
	{
		typedef boost::iostreams::stream<boost::iostreams::array_source> istream_type;
		typedef detail::StreamFactory<istream_type>	factory_type;

		boost::shared_ptr<factory_type>	factory_;

	public:

		IStreamFactory()
			: factory_(new factory_type())
		{
		}


		IStreamPtr	open(ConstBufferRef buf)
		{
			boost::shared_ptr<istream_type> is = factory_->get();

			is->open(buf.data(), buf.size());

			return is;
		}

	};
}

#endif