// *********************************************************************
//  IBufferManager   version:  1.0   ¡¤  date: 09/21/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_IBUFFERMANAGER_HPP
#define LV_IBUFFERMANAGER_HPP

#include <lv/Buffer.hpp>

namespace lv 
{

	class IBufferManager
	{
	public:

		virtual ~IBufferManager(){}

		virtual BufferPtr	get() = 0;
	};

	typedef boost::shared_ptr<IBufferManager> BufferManagerPtr;

}

#endif