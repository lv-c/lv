#include <lv/FrameWork/Net/SSLServer.hpp>
#include <lv/FrameWork/Net/SSLContext.hpp>
#include <lv/SharedPtr.hpp>

#include <boost/asio/ssl.hpp>


namespace lv { namespace net {

	class SSLContextHolder
	{
		asio::ssl::context	ssl_context_;

	public:

		SSLContextHolder(ContextPtr context)
			: ssl_context_(context->service(), asio::ssl::context::sslv23)
		{
		}

		asio::ssl::context &	get()
		{
			return ssl_context_;
		}
	};

	SSLServer::SSLServer(ContextPtr context, creator_type session_creator /* = creator_type */,
			std::string const & password /* = "rswvfbuk" */)
		: base_type(context, session_creator)
		, ssl_context_(new SSLContextHolder(context))
		, password_(password)
	{
		boost::dynamic_pointer_cast<SSLContext>(context_)->set_ssl_context(lv::shared_from_object(ssl_context_->get()));
	}

	/// @exception boost::system::system_error
	void SSLServer::start(unsigned short port)
	{
		init_context();

		base_type::start(port);
	}

	void SSLServer::init_context()
	{
		asio::ssl::context & ctx = ssl_context_->get();

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