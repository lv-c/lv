// *********************************************************************
//  OStreamFactory   version:  1.0   ¡¤  date: 04/14/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


#include <lv/Stream/StreamFactory.hpp>
#include <lv/Buffer.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

namespace lv
{
	class OStreamFactory
	{
		using ostream_type = boost::iostreams::stream<boost::iostreams::back_insert_device<Buffer> >;
		using factory_type = detail::StreamFactory<ostream_type>;

		std::shared_ptr<factory_type>	factory_;

	public:

		OStreamFactory()
			: factory_(std::make_shared<factory_type>())
		{
		}

		using unique_pointer = factory_type::unique_pointer;

		unique_pointer	open(Buffer & buf, std::ios_base::iostate except = std::ios_base::badbit | std::ios_base::failbit)
		{
			auto os = factory_->get();

			os->open(boost::iostreams::back_inserter(buf));
			os->exceptions(except);

			return os;
		}

	};
}