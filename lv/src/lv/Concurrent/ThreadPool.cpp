#include <lv/Concurrent/ThreadPool.hpp>
#include <lv/Concurrent/ThreadName.hpp>
#include <lv/Concurrent/ThreadGroup.hpp>

#include <boost/weak_ptr.hpp>

namespace lv
{
	ThreadPool::ThreadPool(ExceptionHandler const & handler /* = ExceptionHandler() */, bool enable_thread_name /* = true */)
		: exception_handler_(handler)
		, thread_name_enabled_(enable_thread_name)
		, threads_(new ThreadGroup())
	{
	}

	ThreadPool::ServicePtr ThreadPool::create(size_t thread_num /* = 1 */, std::string name /* = std::string() */)
	{
		BOOST_ASSERT(thread_num > 0);

		ServicePtr service(new boost::asio::io_service());

		if(! thread_name_enabled_)
		{
			name.clear();
		}

		for(size_t i = 0; i < thread_num; ++i)
		{
			boost::thread * thread = threads_->create_thread(boost::bind(&ThreadPool::run, name, WeakServicePtr(service),
				exception_handler_));
		}

		return service;
	}

	void ThreadPool::join_all()
	{
		threads_->join_all();
	}

	void ThreadPool::run(std::string name, WeakServicePtr weak_service, ExceptionHandler handler)
	{
		if(! name.empty())
		{
			lv::set_current_thread_name(name.c_str());
		}

		try
		{
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
		catch(std::exception const & ex)
		{
			if(handler)
			{
				handler(name, ex);
			}
			else
			{
				throw;
			}
		}
	}
}