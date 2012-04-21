#include <lv/FrameWork/Net/PacketProxy.hpp>

namespace lv { namespace net {

	PacketProxy::PacketProxy(BufferPtr buf, PacketProxyCallback const & callback)
		: callback_(callback)
		, bos_(buf)
		, valid_(true)
	{
	}

	PacketProxy::PacketProxy(PacketProxy & rhs)
		: callback_(rhs.callback_)
		, bos_(rhs.bos_.buffer_ptr())
		, valid_(true)
	{
		bos_.switch_endian(rhs.bos_.switch_endian());
		rhs.valid_ = false;
	}

	PacketProxy::~PacketProxy()
	{
		if(valid_)
		{
			callback_(bos_.buffer_ptr());
		}
	}

	void PacketProxy::switch_endian(bool s)
	{
		bos_.switch_endian(s);
	}

} }