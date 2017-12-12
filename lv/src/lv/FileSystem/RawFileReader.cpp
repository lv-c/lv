#include <lv/FileSystem/RawFileReader.hpp>
#include <lv/Exception.hpp>

#include <boost/filesystem/operations.hpp>

#include <fstream>

namespace lv
{

	RawFileReader::RawFileReader(std::string const & working_dir /* = std::string() */)
		: IFileIO(working_dir)
	{
	}

	void RawFileReader::fulfill(std::string const & file, BufferPtr buf)
	{
		std::string path = resolve(file);

		std::ifstream infile(path, std::ios_base::binary);
		if (!infile)
		{
			throw std::system_error(std::make_error_code(std::errc::io_error), "error opening file: " + file);
		}

		// get the size of the file
		infile.seekg(0, std::ios_base::end);
		std::streamsize len = infile.tellg();
		if (len != 0)
		{
			// read the whole file
			infile.seekg(0, std::ios_base::beg);

			init_buffer(*buf, static_cast<size_t>(len));
			infile.read(buf->data(), len);
		}
		else
		{
			buf->clear();
		}

		if (!infile)
		{
			throw std::system_error(std::make_error_code(std::errc::io_error), "error reading file : " + file);
		}
	}

	bool RawFileReader::exist(std::string const & file)
	{
		return boost::filesystem::exists(resolve(file));
	}
}
