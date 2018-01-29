// *********************************************************************
//  IStreamFactory   version:  1.0   ¡¤  date: 04/14/2010
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
#include <lv/Buffer.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

namespace lv
{
	class IStreamFactory
	{
		using istream_type = boost::iostreams::stream<boost::iostreams::array_source>;
		using factory_type = detail::StreamFactory<istream_type>;

		std::shared_ptr<factory_type>	factory_;

	public:

		IStreamFactory()
			: factory_(std::make_shared<factory_type>())
		{
		}

		using unique_pointer = factory_type::unique_pointer;

		unique_pointer	open(ConstBufferRef buf, 
			std::ios_base::iostate except = std::ios_base::badbit | std::ios_base::failbit)
		{
			auto is = factory_->get();

			is->open(buf.data(), buf.size());
			is->exceptions(except);

			return is;
		}

	};
}

#endif