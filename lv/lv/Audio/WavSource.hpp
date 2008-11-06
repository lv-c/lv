// *********************************************************************
//  WavSource   version:  1.0   ¡¤  date: 10/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_WAVSOURCE_HPP
#define LV_WAVSOURCE_HPP

#include <lv/Audio/AudioDataSource.hpp>
#include <lv/StreamPtr.hpp>

namespace lv
{
	class WavSource : public AudioDataSource
	{

		IStreamPtr	istream_;

		std::ios_base::off_type	data_offset_;

	public:

		/**
		 * @exception InvalidAudioData
		 * @exception UnSupportedAudioFormat
		 */
		WavSource(IStreamPtr is);

		virtual	size_t	read(BufferRef buf);

		virtual	void	reset();

	};
}

#endif // LV_WAVSOURCE_HPP