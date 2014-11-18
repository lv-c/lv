// *********************************************************************
//  ServiceWrapper   version:  1.0   ¡¤  date: 2012/03/28
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERVICEWRAPPER_HPP
#define LV_SERVICEWRAPPER_HPP

#include <boost/assert.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include <boost/shared_ptr.hpp>

namespace lv
{
	class ServiceWrapper
	{
		boost::asio::io_service * service_;

		boost::asio::strand * strand_;

	public:

		ServiceWrapper()
			: service_(NULL)
			, strand_(NULL)
		{
		}

		ServiceWrapper(boost::asio::io_service & service)
			: service_(&service)
			, strand_(NULL)
		{
		}

		ServiceWrapper(boost::asio::strand & strand)
			: service_(NULL)
			, strand_(&strand)
		{
		}

		operator bool() const
		{
			return (service_ != NULL || strand_ != NULL);
		}

		boost::asio::io_service &	service() const
		{
			if(service_ != NULL)
			{
				return *service_;
			}
			else
			{
				return strand_->get_io_service();
			}
		}

		bool	has_strand() const
		{
			return (strand_ != NULL);
		}

		boost::asio::strand &	strand() const
		{
			BOOST_ASSERT(strand_ != NULL);
			return *strand_;
		}

		template<class Handler>
		void	post(Handler const & handler) const
		{
			BOOST_ASSERT(*this);

			if(service_ != NULL)
			{
				service_->post(handler);
			}
			else
			{
				strand_->post(handler);
			}
		}
	};
}

#endif