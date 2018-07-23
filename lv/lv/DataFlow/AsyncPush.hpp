// *********************************************************************
//  AsyncPush   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/IOContextWrapper.hpp>
#include <lv/DataFlow/PushPolicyBase.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>


namespace lv::flow
{
	template<class T>
	class AsyncPush : public PushPolicyBase<T>
	{

		IOContextWrapper	io_wrapper_;

	public:

		AsyncPush(boost::asio::io_context & io)
			: io_wrapper_(io)
		{
		}

		AsyncPush(boost::asio::io_context::strand & strand)
			: io_wrapper_(strand)
		{
		}

		void operator () (T t)
		{
			io_wrapper_.post([this, t = std::move(t)] { callback_(t); });
		}
	};

}
