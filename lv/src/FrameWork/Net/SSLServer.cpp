#include <lv/FrameWork/Net/SSLServer.hpp>
#include <lv/FrameWork/Net/SSLContext.hpp>
#include <lv/SharedPtr.hpp>

#include <boost/asio/ssl.hpp>


namespace lv { namespace net {

	SSLServer::SSLServer(ContextPtr context, creator_type session_creator /* = creator_type */,
			std::string const & password /* = "rswvfbuk" */)
		: base_type(context, session_creator)
		, password_(password)
	{
	}

	/// @exception boost::system::system_error
	void SSLServer::start(unsigned short port)
	{
		init_context();

		base_type::start(port);
	}

	void SSLServer::init_context()
	{
		asio::ssl::context & ctx = * boost::shared_dynamic_cast<SSLContext>(context_)->ssl_context();

		ctx.set_options(
			asio::ssl::context::default_workarounds
			| asio::ssl::context::no_sslv2
			| asio::ssl::context::single_dh_use);

		ctx.set_password_callback(boost::bind(&SSLServer::get_password, this));
		ctx.use_certificate_file("cert/rscert.pem", asio::ssl::context::pem);
		ctx.use_private_key_file("cert/rsprivkey.pem", asio::ssl::context::pem);
		ctx.use_tmp_dh_file("cert/dh256.pem");
	}

	std::string SSLServer::get_password()
	{
		return password_;
	}

} }