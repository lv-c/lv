#include <lv/FrameWork/Net/PacketProxy.hpp>

namespace lv { namespace net {

	PacketProxy::PacketProxy(BufferPtr buf, PacketProxyCallback const & callback)
		: callback_(callback)
		, raw_os_(buf)
		, bos_(raw_os_)
		, valid_(true)
	{
	}

	PacketProxy::PacketProxy(PacketProxy & rhs)
		: callback_(rhs.callback_)
		, raw_os_(rhs.raw_os_.buffer_ptr())
		, bos_(raw_os_)
		, valid_(true)
	{
		bos_.switch_endian(rhs.bos_.switch_endian());
		rhs.valid_ = false;
	}

	PacketProxy::~PacketProxy()
	{
		if(valid_)
		{
			callback_(raw_os_.buffer_ptr());
		}
	}

	void PacketProxy::switch_endian(bool s)
	{
		bos_.switch_endian(s);
	}

} }