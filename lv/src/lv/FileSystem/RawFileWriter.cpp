#include <lv/FileSystem/RawFileWriter.hpp>
#include <fstream>
#include <lv/Exception.hpp>

namespace lv
{
	void RawFileWriter::fulfill(std::string const & file, BufferPtr buf)
	{
		std::ofstream ofile(file.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if(! ofile)
			throw(file_io_error("error opening file: " + file));
		
		// get the size of the file
		if(buf->size() != 0)
			ofile.write(buffer::data(buf), buf->size());	// write the whole file

		if(! ofile.good())
			throw(file_io_error("error writing file: " + file));
	}
}