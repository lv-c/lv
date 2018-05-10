// *********************************************************************
//  Context   version:  1.0   ¡¤  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/ServiceWrapper.hpp>


namespace lv::net
{
	class Context
	{
	protected:

		ServiceWrapper		service_wrapper_;

	public:

		explicit Context(ServiceWrapper const & service_wrapper)
			: service_wrapper_(service_wrapper)
		{
		}

		virtual	~Context() = default;

		asio::io_service &	service() const
		{
			return service_wrapper_.service();
		}

		bool	has_strand() const
		{
			return service_wrapper_.has_strand();
		}

		asio::strand &	strand() const
		{
			return service_wrapper_.strand();
		}

		ServiceWrapper const &	service_wrapper() const
		{
			return service_wrapper_;
		}
	};

}
