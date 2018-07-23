#include <lv/FileSystem/IFileIO.hpp>

#include <filesystem>


namespace lv
{
	IFileIO::IFileIO(std::string const & working_dir /* = std::string() */)
		: working_dir_(working_dir)
	{
	}

	void IFileIO::init_buffer(Buffer & buf, size_t len)
	{
		buf.resize(len + 1);
		buf[len] = '\0';
		buf.resize(len);
	}

	std::string IFileIO::resolve(std::string const & file)
	{
		std::filesystem::path path(file);

		if (!path.has_root_directory())
		{
			path = working_dir_ / path;
		}

		return path.string();
	}
}