// *********************************************************************
//  RpcBase   version:  1.0   ¡¤  date: 12/05/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPCBASE_HPP
#define LV_RPCBASE_HPP

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>

#include <lv/rpc/Fwd.hpp>
#include <lv/rpc/IBufferManager.hpp>
#include <lv/rpc/ISocket.hpp>

namespace lv { namespace rpc {

	typedef boost::function<void(BufferPtr)>	BufProcessor;

	class RpcBase : boost::noncopyable
	{
		typedef boost::mutex::scoped_lock	scoped_lock;
		boost::mutex	mutex_;		// private. for processors

		BufferManagerPtr	buf_manager_;

		BufProcessor		prepro_;
		BufProcessor		postpro_;

	protected:

		RpcBase(BufferManagerPtr buf_manger)
			: buf_manager_(buf_manger)
		{
		}

		BufferPtr	get_buffer()
		{
			BufferPtr buf = buf_manager_->get();

			scoped_lock lock(mutex_);
			if(prepro_)
				prepro_(buf);

			return buf;
		}

		void	send_buffer(BufferPtr buf, ISocket & sock)
		{
			{
				scoped_lock lock(mutex_);
				if(postpro_)
					postpro_(buf);
			}
			
			sock.send(buf);
		}

	public:

		BufferManagerPtr	buffer_manager() const
		{
			return buf_manager_;
		}

		void	set_buf_preprocessor(BufProcessor pro)
		{
			scoped_lock lock(mutex_);
			this->prepro_ = pro;
		}

		void	set_buf_postprocessor(BufProcessor pro)
		{
			scoped_lock lock(mutex_);
			this->postpro_ = pro;
		}
	};

} }

#endif // LV_RPCBASE_HPP