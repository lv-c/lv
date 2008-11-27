// *********************************************************************
//  DSSoundBuffer   version:  1.0   ¡¤  date: 11/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DSSOUNDBUFFER_HPP
#define LV_DSSOUNDBUFFER_HPP

#include <vector>
#include <boost/shared_ptr.hpp>
#include <dsound.h>

#include <lv/Audio/AudioBuffer.hpp>

namespace lv
{
	class AudioDataSource;

	class DSSoundBuffer : public SoundBuffer
	{

		typedef boost::shared_ptr<IDirectSoundBuffer>	dsbuffer_ptr;
		typedef std::vector<dsbuffer_ptr>	buffer_vec;

		buffer_vec	buffers_;

	public:

		DSSoundBuffer(AudioDataSource & data, size_t buf_num);


		virtual	void	play(bool loop = false);

		virtual	void	stop();

		virtual	void	reset();

		virtual	void	set_volume(float vol);
		
		virtual	bool	playing() const;

	};
}

#endif // LV_DSSOUNDBUFFER_HPP