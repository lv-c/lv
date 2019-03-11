#include <lv/FrameWork/Net/DoubleBuffer.hpp>


namespace lv::net
{

	DoubleBuffer::ScopedLock::ScopedLock(DoubleBuffer * obj, Buffer const * buf)
		: obj_(obj)
		, buf_(buf)
	{
	}

	DoubleBuffer::ScopedLock::ScopedLock(ScopedLock && other)
		: obj_(other.obj_)
		, buf_(other.buf_)
	{
		other.obj_ = nullptr;
		other.buf_ = nullptr;
	}

	DoubleBuffer::ScopedLock::~ScopedLock()
	{
		if (buf_ != nullptr)
		{
			obj_->unlock(*buf_);
		}
	}

	DoubleBuffer::ScopedLock::operator bool() const
	{
		return (buf_ != nullptr);
	}

	Buffer const * DoubleBuffer::ScopedLock::buffer() const
	{
		return buf_;
	}

	DoubleBuffer::DoubleBuffer()
	{
		reset();
	}

	//
	void DoubleBuffer::reset()
	{
		std::lock_guard<std::mutex> lock(mutex_);

		for (Buffer & buf : buffers_)
		{
			buf.clear();
		}

		write_index_ = 0;
		lock_index_ = NullLock;

		shutdown_ = false;
	}

	void DoubleBuffer::put(ConstBufferRef buf)
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (!shutdown_)
		{
			buffer::append(buffers_[write_index_], buf);
		}
	}

	void DoubleBuffer::shutdown()
	{
		std::lock_guard<std::mutex> lock(mutex_);

		shutdown_ = true;
	}

	Buffer const * DoubleBuffer::lock(bool * need_shutdown /* = nullptr */)
	{
		std::lock_guard<std::mutex> lock(mutex_);

		Buffer const * ret = nullptr;
		bool ns = false;

		if (lock_index_ == NullLock)
		{
			if (!buffers_[write_index_].empty())
			{
				lock_index_ = write_index_;
				write_index_ = 1 - write_index_;

				ret = &buffers_[lock_index_];
			}
			else
			{
				ns = shutdown_;
			}
		}

		if (need_shutdown != nullptr)
		{
			*need_shutdown = ns;
		}

		return ret;
	}

	void DoubleBuffer::unlock(Buffer const & buf)
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (lock_index_ != NullLock && &buffers_[lock_index_] == &buf)
		{
			buffers_[lock_index_].clear();
			lock_index_ = NullLock;
		}
		else
		{
			BOOST_ASSERT(false);
		}
	}

	DoubleBuffer::ScopedLock DoubleBuffer::scope_lock()
	{
		return ScopedLock(this, lock());
	}

}