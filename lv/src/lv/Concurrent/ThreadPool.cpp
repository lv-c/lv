#include <lv/Concurrent/ThreadPool.hpp>


namespace lv
{

	ThreadPool::ThreadPool(unsigned thread_count /* = std::thread::hardware_concurrency() */)
	{
		work_ = std::make_unique<work_type>(service_);

		threads_.reserve(thread_count);

		for (unsigned i = 0; i < thread_count; ++i)
		{
			threads_.push_back(std::thread([this] {
				service_.run();
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

		service_.stop();
	}

	boost::asio::io_service & ThreadPool::service()
	{
		return service_;
	}

	size_t ThreadPool::size() const
	{
		return threads_.size();
	}

}