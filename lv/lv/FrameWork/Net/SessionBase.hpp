// *********************************************************************
//  SessionBase   version:  1.0   ��  date: 01/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SESSIONBASE_HPP
#define LV_NET_SESSIONBASE_HPP

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/FrameWork/Net/Context.hpp>


#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace lv { namespace net {

	class SessionBase : public boost::enable_shared_from_this<SessionBase>
	{
	protected:

		Context	context_;

	public:

		SessionBase(Context const & context)
			: context_(context)
		{
			
		}

	protected:

		virtual	asio::ip::tcp::socket & socket() = 0;

		virtual	void	on_error(boost::system::error_code const & error)
		{
		}

		void	on_connected_internal()
		{
			start_read();
			on_connected();
		}

		virtual	void	on_connected() {}

		virtual	void	on_receive(BufferPtr buf) {}

		virtual	void	close()
		{
			boost::system::error_code error;
			socket().close(error);
		}

		virtual void	start_read()
		{
			BufferPtr buf = context_.buffer();

			buf->resize(buf->capacity());

			if(context_.strand())
			{
				socket().async_read_some(asio::buffer(*buf), context_.strand()->wrap(
					boost::bind(&SessionBase::handle_read, shared_from_this(), buf,
					asio::placeholders::bytes_transferred, asio::placeholders::error)));
			}
			else
			{
				socket().async_read_some(asio::buffer(*buf), 
					boost::bind(&SessionBase::handle_read, shared_from_this(), buf,
					asio::placeholders::bytes_transferred, asio::placeholders::error));
			}
			
		}

		virtual	void	start_write(BufferPtr buf)
		{
			if(context_.strand())
			{
				boost::asio::async_write(socket(), asio::buffer(*buf), context_.strand()->wrap(
					boost::bind(&SessionBase::handle_write, shared_from_this(), 
					buf, asio::placeholders::error)));
			}
			else
			{
				boost::asio::async_write(socket(), asio::buffer(*buf), 
					boost::bind(&SessionBase::handle_write, shared_from_this(), 
					buf, asio::placeholders::error));
			}
			
		}


		virtual	void	handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error)
		{
			if(error)
			{
				on_error(error);
			}
			else
			{
				buf->resize(bytes_transferred);
				on_receive(buf);

				start_read();
			}
		}


		virtual	void	handle_write(BufferPtr buf, boost::system::error_code const & error)
		{
			if(error)
			{
				on_error(error);
			}
		}
		

	};

} }

#endif