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

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/FrameWork/Net/Fwd.hpp>

#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>

#include <memory>


namespace lv { namespace net {

	class AcceptorHolder;

	template<class SessionType>
	struct SessionCreator
	{
		SessionPtr	operator() (ContextPtr context) const
		{
			return std::make_shared<SessionType>(context);
		}
	};

	class ServerBase : boost::noncopyable
	{

		typedef boost::signals2::signal<void(SessionPtr)>	NewSessionEvent;

		std::unique_ptr<AcceptorHolder>	acceptor_;

		NewSessionEvent	new_session_event_;

	protected:

		ContextPtr	context_;

		typedef std::function<SessionPtr(ContextPtr)>	creator_type;
		creator_type	session_creator_;

	public:

		/// you can either pass in a session_creator function or overload create_session
		///		to create a new session
		ServerBase(ContextPtr context, creator_type session_creator = creator_type());

		virtual	~ServerBase();

		virtual	void	start(unsigned short port, std::string const & to_bind = std::string());

		virtual	void	close();

		NewSessionEvent &	new_session_event();

	protected:

		virtual	void	handle_accept(SessionPtr session, boost::system::error_code const & error);

		virtual	void	start_accept();

		virtual	SessionPtr	create_session();

		virtual	void	on_new_session(SessionPtr session);
	};

} }


#endif
