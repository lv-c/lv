// *********************************************************************
//  ServerBase   version:  1.0   ��  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SERVERBASE_HPP
#define LV_NET_SERVERBASE_HPP

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/FrameWork/Net/Context.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

namespace lv { namespace net {


	template<class S>
	class ServerBase : boost::noncopyable
	{
	protected:

		typedef	S	session_type;
		typedef boost::shared_ptr<session_type>	session_ptr;

		boost::scoped_ptr<asio::ip::tcp::acceptor>	acceptor_;

		ContextPtr	context_;

	public:

		/// you can either pass in a session_creator function or overload create_session
		///		to create a new session
		ServerBase(ContextPtr context)
			: context_(context)
		{
		}

		virtual	~ServerBase()
		{
		}

		virtual	void	start(unsigned short port)
		{
			acceptor_.reset(new asio::ip::tcp::acceptor(context_->service(), 
				asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)));

			start_accept();
		}

		virtual	void	close()
		{
			boost::system::error_code error;
			acceptor_->close(error);
			acceptor_.reset();
		}

	protected:

		virtual	void	handle_accept(session_ptr session, boost::system::error_code const & error)
		{
			if(! error)
			{
				on_new_session(session);

				start_accept();
			}
		}

		virtual	void	start_accept()
		{
			session_ptr	session = create_session();

			acceptor_->async_accept(session->socket(), 
				boost::bind(&ServerBase::handle_accept, this, session,
				asio::placeholders::error));
		}

		virtual	session_ptr	create_session()
		{
			return session_ptr(new session_type(context_));
		}

		virtual	void	on_new_session(session_ptr session)
		{
			session->start();
		}

	};

} }


#endif
