// *********************************************************************
//  CsSession   version:  1.0   ¡¤  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_CSSESSION_HPP
#define LV_NET_CSSESSION_HPP

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/FrameWork/Net/SessionBase.hpp>

namespace lv { namespace net {

	struct ServerSide {};
	struct ClientSide {};

	template<typename Side>
	class CsSession;

	template<>
	class CsSession<ServerSide> : public SessionBase
	{
	public:

		explicit CsSession(ContextPtr context)
			: SessionBase(context)
		{
		}

		void	start()
		{
			on_connected_internal();
		}
	};


	template<>
	class CsSession<ClientSide> : public SessionBase
	{
	public:
		
		explicit CsSession(ContextPtr context)
			: SessionBase(context)
		{
		}

		/// @exception runtime_error
		virtual	void	start(std::string const & ip, std::string const & port)
		{
			asio::ip::tcp::resolver::query query(ip, port);
			asio::ip::tcp::resolver resolver(context_->service());

			if(context_->strand())
			{
				socket().async_connect(*resolver.resolve(query), 
					context_->strand()->wrap(
					boost::bind(&CsSession::handle_connect, 
					boost::shared_dynamic_cast<CsSession>(shared_from_this()),
					asio::placeholders::error)));
			}
			else
			{
				socket().async_connect(*resolver.resolve(query), 
					boost::bind(&CsSession::handle_connect, 
					boost::shared_dynamic_cast<CsSession>(shared_from_this()),
					asio::placeholders::error));
			}
			
		}

	protected:

		virtual	void	handle_connect(boost::system::error_code const & error)
		{
			if(! error)
			{
				on_connected_internal();
			}
			else
			{
				on_error(ErrorConnect, error);
			}
		}

	};

} }

#endif