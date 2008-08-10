#include <lv/FileSystem/IFileIO.hpp>
#include <lv/FileSystem/IOTask.hpp>
#include <lv/NullDeleter.hpp>


namespace lv
{
	IOFuture IFileIO::add_task(std::string const & file, BufferPtr buffer)
	{
		IOTask task(shared_from_object(file), buffer, this->shared_from_this());
		IOFuture future(task);

		task();
		return future;
	}
}