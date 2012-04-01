// *********************************************************************
//  Context   version:  1.0   ��  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_CONTEX_HPP
#define LV_NET_CONTEX_HPP

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/ServiceWrapper.hpp>

#include <lv/IBufferManager.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

namespace lv { namespace net {

	class Context
	{
	protected:

		BufferManagerPtr	buf_manager_;

		ServiceWrapper		service_wrapper_;

	public:

		Context(BufferManagerPtr buf_manager, ServiceWrapper const & service_wrapper)
			: buf_manager_(buf_manager)
			, service_wrapper_(service_wrapper)
		{
		}

		virtual	~Context()
		{
		}

		
		BufferPtr	buffer() const
		{
			return buf_manager_->get();
		}

		BufferManagerPtr	buffer_manager() const
		{
			return buf_manager_;
		}

		asio::io_service &	service() const
		{
			return service_wrapper_.service();
		}

		bool	has_strand() const
		{
			return service_wrapper_.has_strand();
		}

		boost::asio::strand &	strand() const
		{
			return service_wrapper_.strand();
		}
	};

} }

#endif
