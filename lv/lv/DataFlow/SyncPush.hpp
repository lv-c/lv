// *********************************************************************
//  SyncPush   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/DataFlow/PushPolicyBase.hpp>

namespace lv::flow
{
	template<class T>
	class SyncPush : public PushPolicyBase<T>
	{
	public:

		void operator () (T const & t)
		{
			this->callback_(t);
		}
	};

} 
