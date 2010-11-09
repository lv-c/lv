// *********************************************************************
//  PacketProxy   version:  1.0   ��  date: 10/14/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_PACKETPROXY_HPP
#define LV_NET_PACKETPROXY_HPP

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/Stream/OBufferStream.hpp>
#include <lv/BinaryStream/BinaryOStream.hpp>

#include <boost/function.hpp>

namespace lv { namespace net {

	typedef boost::function<void(BufferPtr)> PacketProxyCallback;

	class PacketProxy
	{
		// raw_os_ should be declared before bos_

		OBufferStream	raw_os_;
		BinaryOStream	bos_;

		bool	valid_;

		PacketProxyCallback	callback_;

	public:

		PacketProxy(BufferPtr buf, PacketProxyCallback const & callback);

		PacketProxy(PacketProxy & rhs);

		~PacketProxy();

		template<typename T>
		PacketProxy & operator << (T const & t)
		{
			BOOST_ASSERT(valid_);

			bos_ << t;
			return *this;
		}
	};

} }

#endif