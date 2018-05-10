#include <lv/FrameWork/Net/PacketProxy.hpp>


namespace lv::net
{
	PacketProxy::PacketProxy(BufferPtr buf, PacketProxyCallback callback)
		: bos_(buf)
		, valid_(true)
		, callback_(std::move(callback))
	{
	}

	PacketProxy::PacketProxy(PacketProxy && rhs)
		: bos_(rhs.bos_.buffer_ptr())
		, valid_(true)
		, callback_(rhs.callback_)
	{
		bos_.switch_endian(rhs.bos_.switch_endian());
		rhs.valid_ = false;
	}

	PacketProxy::~PacketProxy()
	{
		if (valid_)
		{
			callback_(bos_.buffer_ptr());
		}
	}

	void PacketProxy::switch_endian(bool s)
	{
		bos_.switch_endian(s);
	}

}
