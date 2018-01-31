// *********************************************************************
//  IBufferManager   version:  1.0   ¡¤  date: 09/21/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Buffer.hpp>

namespace lv 
{

	class IBufferManager
	{
	public:

		virtual ~IBufferManager(){}

		virtual BufferPtr	get() = 0;
	};

	using BufferManagerPtr = std::shared_ptr<IBufferManager>;

}