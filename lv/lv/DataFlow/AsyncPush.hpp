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

#include <boost/bind.hpp>

namespace lv { namespace flow {

	template<class T>
	class AsyncPush : public PushPolicyBase<T>
	{

		ServiceWrapper	service_wrapper_;

	public:

		AsyncPush(ServiceWrapper const & service_wrapper)
			: service_wrapper_(service_wrapper)
		{
		}

		void operator () (T const & t)
		{
			service_wrapper_->post(boost::bind(callback_, t));
		}
	};

} }

#endif