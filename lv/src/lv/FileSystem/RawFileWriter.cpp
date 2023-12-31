#include <lv/FileSystem/RawFileWriter.hpp>
#include <lv/Exception.hpp>
#include <lv/Ensure.hpp>

#include <fstream>
#include <filesystem>


namespace lv
{
	RawFileWriter::RawFileWriter(std::string const & working_dir /* = std::string() */)
		: IFileIO(working_dir)
	{
	}

	void RawFileWriter::fulfill(std::string const & file, BufferPtr buf)
	{
		std::string path = resolve(file);

		std::ofstream ofile(path, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		LV_ENSURE(ofile, std::system_error(std::make_error_code(std::errc::io_error), "error opening file: " + file));
		
		// get the size of the file
		if (buf->size() != 0)
		{
			ofile.write(buf->data(), buf->size());	// write the whole file
		}

		LV_ENSURE(ofile, std::system_error(std::make_error_code(std::errc::io_error), "error writing file: " + file));
	}

	bool RawFileWriter::exist(std::string const & file)
	{
		return std::filesystem::exists(resolve(file));
	}
}