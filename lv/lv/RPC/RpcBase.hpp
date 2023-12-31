// *********************************************************************
//  RpcBase   version:  1.0   ��  date: 12/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


#include <lv/RPC/Fwd.hpp>

#include <lv/IBufferManager.hpp>
#include <lv/Stream/IStreamFactory.hpp>
#include <lv/Stream/OStreamFactory.hpp>
#include <lv/Serialization/IArchiveWrapper.hpp>
#include <lv/Serialization/OArchiveWrapper.hpp>

#include <boost/noncopyable.hpp>


namespace lv::rpc
{
	class RpcBase : boost::noncopyable
	{
		BufferManagerPtr	buf_manager_;

	protected:

		OStreamFactory	ostream_factory_;

		IStreamFactory	istream_factory_;

	protected:

		RpcBase(BufferManagerPtr buf_manger)
			: buf_manager_(buf_manger)
		{
		}

		BufferPtr	get_buffer()
		{
			return buf_manager_->get();
		}

	public:

		BufferManagerPtr	buffer_manager() const
		{
			return buf_manager_;
		}
	};

}