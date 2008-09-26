// *********************************************************************
//  ISocket   version:  1.0   ¡¤  date: 09/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_ISOCKET_HPP
#define LV_RPC_ISOCKET_HPP

namespace lv { namespace rpc {

	class ISocket
	{
	public:

		virtual bool	connected() const = 0;

		virtual	BufferPtr	preprocess(BufferPtr buf) 
		{
			return buf;
		}

		virtual	void	send(BufferPtr buf) = 0;
	};

} }

#endif // LV_RPC_ISOCKET_HPP