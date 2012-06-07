// *********************************************************************
//  OStreamFactory   version:  1.0   ¡¤  date: 04/14/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAM_OSTREAMFACTORY_HPP
#define LV_STREAM_OSTREAMFACTORY_HPP


#include <lv/Stream/StreamFactory.hpp>
#include <lv/StreamPtr.hpp>
#include <lv/Buffer.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

namespace lv
{
	class OStreamFactory
	{
		typedef boost::iostreams::stream<boost::iostreams::back_insert_device<Buffer> > ostream_type;
		typedef detail::StreamFactory<ostream_type>	factory_type;

		boost::shared_ptr<factory_type>	factory_;

	public:

		OStreamFactory()
			: factory_(new factory_type())
		{
		}


		OStreamPtr	open(Buffer & buf, std::ios_base::iostate except = std::ios_base::badbit | std::ios_base::failbit)
		{
			boost::shared_ptr<ostream_type> os = factory_->get();

			os->open(boost::iostreams::back_inserter(buf));
			os->exceptions(except);

			return os;
		}

	};
}

#endif