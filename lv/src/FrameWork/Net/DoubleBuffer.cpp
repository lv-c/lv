#include <lv/FrameWork/Net/DoubleBuffer.hpp>

#include <boost/assert.hpp>


namespace lv::net
{

	DoubleBuffer::DoubleBuffer()
	{
		reset();
	}

	void DoubleBuffer::write(ConstBufferRef data)
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (!shutdown_)
		{
			buffer::append(buffers_[write_index_], data);
		}
	}

	Buffer const * DoubleBuffer::start_read(bool * need_shutdown /* = nullptr */)
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

	void DoubleBuffer::end_read(Buffer const & buf)
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

	DoubleBuffer::ScopedRead DoubleBuffer::scoped_read()
	{
		return ScopedRead(this, start_read());
	}

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

	void DoubleBuffer::shutdown()
	{
		std::lock_guard<std::mutex> lock(mutex_);

		shutdown_ = true;
	}
}