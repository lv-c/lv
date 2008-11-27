// *********************************************************************
//  DSMusicBuffer   version:  1.0   ¡¤  date: 11/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DSMUSICBUFFER_HPP
#define LV_DSMUSICBUFFER_HPP

#include <lv/Audio/AudioBuffer.hpp>
#include <lv/Audio/AudioDataSource.hpp>

namespace lv
{
	class DSMusicBuffer : public MusicBuffer
	{
		AudioDataSourcePtr data_;
	public:

		DSMusicBuffer(AudioDataSourcePtr data);


		virtual	void	play(bool loop = false);

		virtual	void	stop();

		virtual	void	reset();

		virtual	void	set_volume(float vol);

		virtual	bool	playing() const;

	};
}

#endif // LV_DSMUSICBUFFER_HPP