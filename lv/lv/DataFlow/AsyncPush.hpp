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

#include <lv/DataFlow/PushPolicyBase.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include <boost/bind.hpp>

namespace lv { namespace flow {

	template<class T>
	class AsyncPush : public PushPolicyBase<T>
	{

		boost::asio::io_service * service_;

		boost::asio::strand * strand_;

	public:

		AsyncPush(boost::asio::io_service & service)
			: service_(&service)
			, strand_(NULL)
		{
		}

		AsyncPush(boost::asio::strand & strand)
			: service_(NULL)
			, strand_(&strand)
		{
		}

		void operator () (T const & t)
		{
			if(strand_ != NULL)
			{
				strand_->post(boost::bind(callback_, t));
			}
			else
			{
				service_->post(boost::bind(callback_, t));
			}
		}
	};

} }

#endif