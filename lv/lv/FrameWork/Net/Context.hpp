// *********************************************************************
//  Context   version:  1.0   ¡¤  date: 01/03/2010
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

#include <lv/IBufferManager.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

namespace lv { namespace net {

	class Context
	{
	protected:

		BufferManagerPtr	buf_manager_;

		typedef boost::shared_ptr<asio::io_service>	service_ptr;
		service_ptr	service_;

		typedef boost::shared_ptr<asio::io_service::strand>	strand_ptr;
		strand_ptr	strand_;

	public:

		Context(BufferManagerPtr buf_manager, service_ptr service)
			: buf_manager_(buf_manager)
			, service_(service)
		{
		}

		Context(BufferManagerPtr buf_manager, strand_ptr strand)
			: buf_manager_(buf_manager)
			, strand_(strand)
		{
		}

		virtual	~Context()
		{
		}

		
		BufferPtr	buffer()
		{
			return buf_manager_->get();
		}

		BufferManagerPtr buffer_manager() const
		{
			return buf_manager_;
		}

		asio::io_service & service() const
		{
			if(strand_)
			{
				return strand_->io_service();
			}
			else
			{
				return *service_;
			}
		}

		strand_ptr strand() const
		{
			return strand_;
		}
	};

} }

#endif
