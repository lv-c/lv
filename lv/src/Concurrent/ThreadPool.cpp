#include <lv/Concurrent/ThreadPool.hpp>
#include <lv/Concurrent/ThreadName.hpp>

#include <boost/weak_ptr.hpp>
#include <boost/thread/thread.hpp>

namespace lv
{
	ThreadPool::ThreadPool(bool enable_thread_name /* = true */)
		: thread_name_enabled_(enable_thread_name)
		, threads_(new boost::thread_group())
	{
	}

	ThreadPool::ServicePtr ThreadPool::create(std::string name /* = std::string */, size_t thread_num /* = 1 */)
	{
		BOOST_ASSERT(thread_num > 0);

		ServicePtr service(new boost::asio::io_service());

		if(! thread_name_enabled_)
		{
			name.clear();
		}

		for(size_t i = 0; i < thread_num; ++i)
		{
			boost::thread * thread = threads_->create_thread(boost::bind(&ThreadPool::run, name, WeakServicePtr(service)));
		}

		return service;
	}

	void ThreadPool::join()
	{
		threads_->join_all();
	}

	void ThreadPool::run(std::string name, WeakServicePtr weak_service)
	{
		if(! name.empty())
		{
			lv::set_current_thread_name(name.c_str());
		}

		while(true)
		{
			ServicePtr service = weak_service.lock();

			if(! service)
			{
				break;
			}

			boost::system::error_code err;
			service->run(err);

			if(err)
			{
				break;
			}

			boost::this_thread::sleep(boost::posix_time::millisec(10));
		}
	}
}