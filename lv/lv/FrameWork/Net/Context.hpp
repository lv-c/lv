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
#include <boost/asio/ssl/context.hpp>

namespace lv { namespace net {

	class Context
	{
		BufferManagerPtr	buf_manager_;

		typedef boost::shared_ptr<asio::io_service>	service_ptr;
		service_ptr	service_;

		typedef boost::shared_ptr<asio::io_service::strand>	strand_ptr;
		strand_ptr	strand_;

		typedef boost::shared_ptr<asio::ssl::context>	ssl_context_ptr;
		ssl_context_ptr	ssl_context_;

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

		void	set_ssl_context(ssl_context_ptr ssl_context)
		{
			this->ssl_context_ = ssl_context;
		}

		ssl_context_ptr	create_ssl_context(asio::ssl::context::method method = asio::ssl::context::sslv23)
		{
			BOOST_ASSERT(! ssl_context_);

			ssl_context_.reset(new asio::ssl::context(service(), method));
			return ssl_context_;
		}

		ssl_context_ptr	get_ssl_context() const
		{
			return ssl_context_;
		}

		BufferPtr	buffer()
		{
			return buf_manager_->get();
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