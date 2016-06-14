#include <lv/FileSystem/IFileIO.hpp>
#include <lv/FileSystem/IOTask.hpp>
#include <lv/SharedPtr.hpp>

#include <boost/filesystem/path.hpp>

namespace lv
{
	IFileIO::IFileIO(std::string const & working_dir /* = std::string() */)
		: working_dir_(working_dir)
	{
	}

	IOFuture IFileIO::add_task(std::string const & file, BufferPtr buf)
	{
		IOTask task(file, buf, this->shared_from_this());

		task();
		return IOFuture(task);
	}

	void IFileIO::init_buffer(Buffer & buf, size_t len)
	{
		buf.resize(len + 1);
		buf[len] = '\0';
		buf.resize(len);
	}

	std::string IFileIO::resolve(std::string const & file)
	{
		boost::filesystem::path path(file);

		if (! path.has_root_directory())
		{
			path = working_dir_ / path;
		}

		return path.string();
	}
}