#include <lv/Concurrent/Async.hpp>
#include <lv/Concurrent/ThreadPool.hpp>


namespace lv::concurrent::detail
{
	ThreadPool &	get_pool()
	{
		static ThreadPool pool;
		return pool;
	}

	boost::asio::io_service & get_service()
	{
		return get_pool().service();
	}

}