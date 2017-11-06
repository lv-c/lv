#include <lv/FrameWork/Net/ServerBase.hpp>
#include <lv/FrameWork/Net/Context.hpp>
#include <lv/FrameWork/Net/SessionBase.hpp>
#include <lv/FrameWork/Net/SocketHolder.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>

#include <functional>


namespace lv::net
{
	class AcceptorHolder
	{
		std::unique_ptr<asio::ip::tcp::acceptor>	acceptor_;

	public:

		AcceptorHolder(ContextPtr context, unsigned short port, std::string const & to_bind)
		{
			asio::ip::address bind_addr;

			if (! to_bind.empty())
			{
				bind_addr = asio::ip::address::from_string(to_bind);
			}

			acceptor_ = std::make_unique<asio::ip::tcp::acceptor>(context->service(), 
				asio::ip::tcp::endpoint(bind_addr, port));
		}

		asio::ip::tcp::acceptor &	get()
		{
			return *acceptor_;
		}
	};

	ServerBase::ServerBase(ContextPtr context, creator_type session_creator /* = creator_type */)
		: context_(context)
		, session_creator_(session_creator)

	{
	}

	ServerBase::~ServerBase()
	{
	}

	void ServerBase::start(unsigned short port, std::string const & to_bind /* = std::string */)
	{
		acceptor_ = std::make_unique<AcceptorHolder>(context_, port, to_bind);

		start_accept();
	}

	void ServerBase::close()
	{
		boost::system::error_code error;
		acceptor_->get().close(error);
		acceptor_.reset();
	}

	ServerBase::NewSessionEvent & ServerBase::new_session_event()
	{
		return new_session_event_;
	}


	void ServerBase::handle_accept(SessionPtr session, boost::system::error_code const & error)
	{
		if (! error)
		{
			on_new_session(session);
			session->server_side_start();

			start_accept();
		}
	}

	void ServerBase::start_accept()
	{
		SessionPtr session = create_session();

		acceptor_->get().async_accept(session->socket()->get(), 
			std::bind(&ServerBase::handle_accept, this, session,
			std::placeholders::_1));
	}

	SessionPtr ServerBase::create_session()
	{
		if (session_creator_)
		{
			return session_creator_(context_);
		}

		BOOST_ASSERT(false);
		return SessionPtr();
	}

	void ServerBase::on_new_session(SessionPtr session)
	{
		new_session_event_(session);
	}

}