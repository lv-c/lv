// *********************************************************************
//  ServerBase   version:  1.0   ¡¤  date: 01/03/2010
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

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace lv { namespace net {


	template<class S>
	class ServerBase
	{
	protected:

		typedef	S	session_type;
		typedef boost::shared_ptr<session_type>	session_ptr;

		asio::io_service & service_;

		asio::ip::tcp::acceptor	acceptor_;

		boost::function<session_ptr()> session_creator_;

	public:

		/// you can either pass in a session_creator function or overload create_session
		///		to create a new session
		ServerBase(asio::io_service & service, unsigned short port, 
			boost::function<session_ptr()> session_creator = boost::function<session_ptr()>())
			: service_(service)
			, acceptor_(service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
			, session_creator_(session_creator)
		{
		}

		virtual	void	start()
		{
			start_accept();
		}

		virtual	void	close()
		{
			boost::system::error_code error;
			acceptor_.close(error);
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

			acceptor_.async_accept(session->socket(), 
				boost::bind(&ServerBase::handle_accept, this, session,
				asio::placeholders::error));
		}

		virtual	session_ptr	create_session()
		{
			if(session_creator_)
			{
				return session_creator_();
			}

			BOOST_ASSERT(false);
			return session_ptr();
		}

		virtual	void	on_new_session(session_ptr session)
		{
			session->start();
		}

	};

} }


#endif
