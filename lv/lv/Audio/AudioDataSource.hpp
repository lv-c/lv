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


#include <lv/Buffer.hpp>
#include <lv/Exception.hpp>

namespace lv
{

	DEFINE_EXCEPTION_MSG(InvalidAudioData, std::runtime_error);
	DEFINE_EXCEPTION_MSG(UnSupportedAudioFormat, std::runtime_error);

	enum AudioFormat
	{
		AF_MONO8,
		AF_MONO16,
		AF_STEREO8,
		AF_STEREO16,
		AF_UNKNOWN
	};

	class AudioDataSource
	{
	protected:

		AudioFormat	format_;

		size_t		freq_;

		size_t		size_;

	public:
		AudioDataSource()
			: format_(AF_UNKNOWN)
			, freq_(0)
			, size_(0)
		{
		}

		virtual size_t	read(BufferRef buf) = 0;

		void	reset() = 0;

		size_t	size() const
		{
			return size_;
		}

		AudioFormat	format() const
		{
			return format_;
		}

		size_t	frequency() const
		{
			return freq_;
		}

	};
}

#endif // LV_AUDIODATASOURCE_HPP