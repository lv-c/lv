// *********************************************************************
//  PushPolicyBase   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <functional>


namespace lv::flow
{
	template<class T>
	class PushPolicyBase
	{
	protected:

		using Callback = std::function<void(T const &)>;

		Callback	callback_;

	public:

		// not intended to be called by the user
		virtual void set_callback(Callback callback)
		{
			this->callback_ = std::move(callback);
		}
	};
}