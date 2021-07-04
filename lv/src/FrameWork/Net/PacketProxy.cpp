#include <lv/FrameWork/Net/PacketProxy.hpp>


namespace lv::net
{
	PacketProxy::PacketProxy(BufferPtr buf, PacketProxyCallback callback)
		: bos_(std::move(buf))
		, callback_(std::move(callback))
	{
	}

	PacketProxy::~PacketProxy()
	{
		callback_(bos_.buffer_ptr());
	}

	void PacketProxy::switch_endian(bool s)
	{
		bos_.switch_endian(s);
	}

}
