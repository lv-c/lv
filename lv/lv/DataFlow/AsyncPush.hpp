// *********************************************************************
//  AsyncPush   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_ASYNCPUSH_HPP
#define LV_DATAFLOW_ASYNCPUSH_HPP

#include <lv/ServiceWrapper.hpp>
#include <lv/DataFlow/PushPolicyBase.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include <functional>


namespace lv::flow
{
	template<class T>
	class AsyncPush : public PushPolicyBase<T>
	{

		ServiceWrapper	service_wrapper_;

	public:

		AsyncPush(boost::asio::io_service & service)
			: service_wrapper_(service)
		{
		}

		AsyncPush(boost::asio::strand & strand)
			: service_wrapper_(strand)
		{
		}

		void operator () (T const & t)
		{
			service_wrapper_.post([this, t] { callback_(t); });
		}
	};

}

#endif
