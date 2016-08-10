// *********************************************************************
//  SyncPush   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_SYNCPUSH_HPP
#define LV_DATAFLOW_SYNCPUSH_HPP

#include <lv/DataFlow/PushPolicyBase.hpp>

namespace lv { namespace flow {

	template<class T>
	class SyncPush : public PushPolicyBase<T>
	{
	public:

		void operator () (T const & t)
		{
			this->callback_(t);
		}
	};

} }

#endif 
