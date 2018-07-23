#include <lv/Concurrent/ThreadPool.hpp>


namespace lv
{

	ThreadPool::ThreadPool(unsigned thread_count /* = std::thread::hardware_concurrency() */)
		: work_(boost::asio::make_work_guard(io_))
	{

		threads_.reserve(thread_count);

		for (unsigned i = 0; i < thread_count; ++i)
		{
			threads_.push_back(std::thread([this] {
				io_.run();
			}));
		}
	}

	ThreadPool::~ThreadPool()
	{
		work_.reset();

		for (std::thread & t : threads_)
		{
			if (t.joinable())
			{
				t.join();
			}
		}

		io_.stop();
	}

	boost::asio::io_context & ThreadPool::io_context()
	{
		return io_;
	}

	size_t ThreadPool::size() const
	{
		return threads_.size();
	}

}