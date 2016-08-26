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

#include <memory>


namespace lv
{
	class ServiceWrapper
	{
		boost::asio::io_service * service_;

		boost::asio::strand * strand_;

	public:

		ServiceWrapper()
			: service_(nullptr)
			, strand_(nullptr)
		{
		}

		ServiceWrapper(boost::asio::io_service & service)
			: service_(&service)
			, strand_(nullptr)
		{
		}

		ServiceWrapper(boost::asio::strand & strand)
			: service_(nullptr)
			, strand_(&strand)
		{
		}

		operator bool() const
		{
			return (service_ != nullptr || strand_ != nullptr);
		}

		boost::asio::io_service &	service() const
		{
			if (service_ != nullptr)
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
			return (strand_ != nullptr);
		}

		boost::asio::strand &	strand() const
		{
			BOOST_ASSERT(strand_ != nullptr);
			return *strand_;
		}

		template<class Handler>
		void	post(Handler && handler) const
		{
			BOOST_ASSERT(*this);

			if (service_ != nullptr)
			{
				service_->post(std::forward<Handler>(handler));
			}
			else
			{
				strand_->post(std::forward<Handler>(handler));
			}
		}
	};
}

#endif