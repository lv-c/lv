// *********************************************************************
//  PushPolicyBase   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_PUSHPOLICYBASE_HPP
#define LV_DATAFLOW_PUSHPOLICYBASE_HPP

#include <boost/function.hpp>

namespace lv { namespace flow {

	template<class T>
	class PushPolicyBase
	{
	protected:

		typedef boost::function<void(T const &)>	Callback;

		Callback	callback_;

	public:

		// not intended to be called by the user
		virtual void set_callback(Callback const & callback)
		{
			this->callback_ = callback;
		}
	};
} }

#endif