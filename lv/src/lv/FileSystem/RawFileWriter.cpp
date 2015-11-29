#include <lv/FileSystem/RawFileWriter.hpp>
#include <lv/Exception.hpp>

#include <fstream>

#include <boost/filesystem/operations.hpp>

namespace lv
{
	RawFileWriter::RawFileWriter(std::string const & working_dir /* = std::string() */)
		: IFileIO(working_dir)
	{
	}

	void RawFileWriter::fulfill(std::string const & file, BufferPtr buf)
	{
		std::string path = resolve(file);

		std::ofstream ofile(path.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if(! ofile)
		{
			throw(file_io_error("error opening file: " + file));
		}
		
		// get the size of the file
		if(buf->size() != 0)
		{
			ofile.write(buffer::data(buf), buf->size());	// write the whole file
		}

		if(! ofile)
		{
			throw(file_io_error("error writing file: " + file));
		}
	}

	bool RawFileWriter::exist(std::string const & file)
	{
		return boost::filesystem::exists(resolve(file));
	}
}