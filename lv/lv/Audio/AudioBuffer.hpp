// *********************************************************************
//  AudioBuffer   version:  1.0   ¡¤  date: 10/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_AUDIOBUFFER_HPP
#define LV_AUDIOBUFFER_HPP

#include <lv/Audio/Vector.hpp>

namespace lv
{
	class AudioBuffer
	{
	public:
		typedef VectorT<float>	vector_type;

		virtual	void	play(bool loop = false) = 0;
	
		virtual	void	stop() = 0;

		virtual	void	reset() = 0;

		virtual	void	set_volume(float vol) = 0;
		virtual	void	set_position(vector_type const & pos)
		{
			this->pos_ = pos;
		}

		virtual	bool	playing() const = 0;

	protected:

		vector_type		pos_;
	};

	class SoundBuffer : public AudioBuffer {};
	class MusicBuffer : public AudioBuffer {};
}

#endif