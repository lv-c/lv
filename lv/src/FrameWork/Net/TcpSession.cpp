#include <lv/FrameWork/Net/TcpSession.hpp>
#include <lv/FrameWork/Net/SocketHolder.hpp>
#include <lv/FrameWork/Net/Context.hpp>

namespace lv { namespace net {

	class TcpSocketHolder : public SocketHolder
	{
		asio::ip::tcp::socket	socket_;

	public:

		TcpSocketHolder(ContextPtr context)
			: socket_(context->service())
		{
		}

		asio::ip::tcp::socket & get() override
		{
			return socket_;
		}
	};

	TcpSession::TcpSession(ContextPtr context)
		: base_type(context, std::make_shared<TcpSocketHolder>(context))
	{
	}

} }