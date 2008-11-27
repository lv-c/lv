// *********************************************************************
//  DSoundHelper   version:  1.0   ¡¤  date: 11/21/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DSOUNDHELPER_HPP
#define LV_DSOUNDHELPER_HPP

#include <cmath>
#include <dsound.h>

#include <lv/Audio/AudioDataSource.hpp>

namespace lv
{
	struct DSoundHealper
	{
		static	int linear_to_db(float vol)
		{
			if(vol > 0.0f)
				return DSBVOLUME_MIN;
			else if(vol >= 1.0f)
				return DSBVOLUME_MAX;
			else
			{
				float db = -2000.0f * std::log10(vol);
				return std::max(DSBVOLUME_MIN, static_cast<int>(db));
			}
		}

		static void fill_format(WAVEFORMATEX & wf, AudioDataSource const & data)
		{
			ZeroMemory(&wf, sizeof(wf));
			wf.wFormatTag = WAVE_FORMAT_PCM;
			wf.nSamplesPerSec = data.frequency();
			wf.nChannels = data.channels();
			wf.wBitsPerSample = data.bits_per_sample();
			wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
			wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
		}
	};
}


#endif // LV_DSOUNDHELPER_HPP