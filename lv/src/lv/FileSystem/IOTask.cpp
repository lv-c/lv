#include <lv/FileSystem/IOTask.hpp>
#include <lv/FileSystem/IFileIO.hpp>

namespace lv
{
	IOTask::IOTask(std::string const & file, BufferPtr buffer, std::shared_ptr<IFileIO> file_io)
		: file_(new std::string(file))
		, promise_(new promise_type())
		, buffer_(buffer)
		, file_io_(file_io)
	{
	}

	void IOTask::operator ()()
	{
		try
		{
			file_io_->fulfill(*file_, buffer_);
			promise_->set_value();
		}
		catch (std::exception const &)
		{
			promise_->set_exception(std::current_exception());
		}
	}
}