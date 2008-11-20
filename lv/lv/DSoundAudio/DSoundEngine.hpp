// *********************************************************************
//  DSoundEngine   version:  1.0   ¡¤  date: 11/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DSOUNDENGINE_HPP
#define LV_DSOUNDENGINE_HPP

#include <dsound.h>

#include <lv/Audio/AudioEngine.hpp>

namespace lv
{
	class DSoundEngine : public AudioEngine
	{

		IDirectSound8 *	dsound_;

	public:

		IDirectSound8 & dsound() const
		{
			return *dsound_;
		}

	};
}

#endif // LV_DSOUNDENGINE_HPP