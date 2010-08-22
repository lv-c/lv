#include <lv/FileSystem/IFileIO.hpp>
#include <lv/FileSystem/IOTask.hpp>
#include <lv/SharedPtr.hpp>


namespace lv
{
	IOFuture IFileIO::add_task(std::string const & file, BufferPtr buf)
	{
		IOTask task(shared_from_object(const_cast<std::string &>(file)), buf, this->shared_from_this());

		task();
		return IOFuture(task);
	}

	void IFileIO::init_buffer(Buffer & buf, size_t len)
	{
		buf.resize(len + 1);
		buf[len] = '\0';
		buf.resize(len);
	}
}