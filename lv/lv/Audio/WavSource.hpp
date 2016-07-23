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

#include <istream>
#include <memory>


namespace lv
{
	class WavSource : public AudioDataSource
	{

		std::shared_ptr<std::istream>	istream_;

		std::streamoff	data_offset_;

	public:

		/**
		 * @exception InvalidAudioData
		 * @exception UnSupportedAudioFormat
		 */
		explicit WavSource(std::shared_ptr<std::istream> is);

		virtual	size_t	read(BufferRef buf);

		virtual	void	reset();

	};
}

#endif
