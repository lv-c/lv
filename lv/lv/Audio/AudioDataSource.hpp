// *********************************************************************
//  AudioDataSource   version:  1.0   ¡¤  date: 10/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_AUDIODATASOURCE_HPP
#define LV_AUDIODATASOURCE_HPP

#include <boost/shared_ptr.hpp>

#include <lv/IntType.hpp>
#include <lv/Buffer.hpp>
#include <lv/Exception.hpp>

namespace lv
{

	DEFINE_EXCEPTION_MSG(InvalidAudioData, std::runtime_error);
	DEFINE_EXCEPTION_MSG(UnSupportedAudioFormat, std::runtime_error);

	// fwd
	class AudioDataSource;
	typedef boost::shared_ptr<AudioDataSource>	AudioDataSourcePtr;

	class AudioDataSource
	{
	protected:

		uint16		channels_;
		uint16		bits_per_sample_;

		size_t		freq_;

		size_t		size_;

	public:
		AudioDataSource()
			: channels_(0)
			, bits_per_sample_(0)
			, freq_(0)
			, size_(0)
		{
		}

		virtual size_t	read(BufferRef buf) = 0;

		virtual void	reset() = 0;

		size_t	size() const
		{
			return size_;
		}

		uint16	channels() const 
		{
			return channels_;
		}
		uint16	bits_per_sample() const
		{
			return bits_per_sample_;
		}

		size_t	frequency() const
		{
			return freq_;
		}

	};
}

#endif