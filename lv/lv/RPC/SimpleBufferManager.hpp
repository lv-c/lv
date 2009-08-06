// *********************************************************************
//  SimpleBufferManager   version:  1.0   ·  date: 12/03/2008
//  --------------------------------------------------------------------
//  boost::bind(&SimpleBufferManager::release, shared_from_this(), _1)
//	比较耗时，如果先保存这个结果，效率提高2倍，但是因为这个结果中保存有
//	shared_ptr<SimpleBufferManager> 的对象，所以这个 SimpleBufferManager
//	永远不会被释放
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SIMPLEBUFFERMANAGER_HPP
#define LV_SIMPLEBUFFERMANAGER_HPP

/// hint : define BOOST_SP_USE_QUICK_ALLOCATOR in your program to increase the 
///		performance and make this class more valuable.

#include <vector>

#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <lv/rpc/IBufferManager.hpp>

namespace lv { namespace rpc {

	class SimpleBufferManager 
		: public IBufferManager
		, public boost::enable_shared_from_this<SimpleBufferManager>
	{
	protected:

		typedef boost::mutex::scoped_lock	scoped_lock;
		boost::mutex	mutex_;

		std::vector<Buffer *>	free_buffers_;

		size_t	init_capacity_;

	public:

		SimpleBufferManager(size_t init_capacity) 
			: init_capacity_(init_capacity) 
		{
		}

		~SimpleBufferManager()
		{
			// lock
			scoped_lock lock(mutex_);

			for(size_t i = 0; i < free_buffers_.size(); ++i)
				delete free_buffers_[i];

			free_buffers_.clear();
		}

		/// return an empty buffer whose capacity is at lease @a init_capacity_
		virtual BufferPtr	get()
		{
			// lock
			scoped_lock lock(mutex_);

			Buffer * buf = NULL;
			if(! free_buffers_.empty())
			{
				buf = free_buffers_.back();
				free_buffers_.pop_back();

				// TODO : this may release the allocated memory.
				buf->clear();

				if(buf->capacity() < init_capacity_)
				{
					BOOST_ASSERT(false);
					buf->reserve(init_capacity_);
				}
			}
			else
			{
				buf = new Buffer();
				buf->reserve(init_capacity_);
			}
			
			return BufferPtr(buf, boost::bind(&SimpleBufferManager::release, 
				shared_from_this(), _1));
		}

	protected:

		virtual void	release(Buffer * buf)
		{
			// lock
			scoped_lock	lock(mutex_);

			free_buffers_.push_back(buf);
		}
	};

} }

#endif // LV_SIMPLEBUFFERMANAGER_HPP