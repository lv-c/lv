#include <lv/FileSystem/IOTask.hpp>
#include <lv/FileSystem/IFileIO.hpp>

namespace lv
{
	IOTask::IOTask(std::string file, BufferPtr buffer, IFileIOPtr file_io)
		: promise_(std::make_shared<promise_type>())
		, file_(std::move(file))
		, buffer_(buffer)
		, file_io_(file_io)
	{
	}

	IOTask::IOTask(IOTask && other)
		: promise_(std::move(other.promise_))
		, file_(std::move(other.file_))
		, buffer_(std::move(other.buffer_))
		, file_io_(std::move(other.file_io_))
	{
	}

	void IOTask::operator ()()
	{
		try
		{
			file_io_->fulfill(file_, buffer_);
		}
		catch (...)
		{
			promise_->set_exception(std::current_exception());
			return;
		}

		promise_->set_value(&buffer_);
	}

	IOFuture IOTask::get_future()
	{
		return promise_->get_future();
	}
}
