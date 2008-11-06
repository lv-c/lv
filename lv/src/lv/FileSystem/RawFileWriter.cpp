#include <lv/FileSystem/RawFileWriter.hpp>
#include <fstream>
#include <lv/Exception.hpp>

namespace lv
{
	void RawFileWriter::fulfill(std::string const & file, BufferPtr buffer)
	{
		std::ofstream ofile(file.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
		if(! ofile)
			throw(file_io_error(std::string("error opening file: ") + file));
		
		// get the size of the file
		if(buffer->size() != 0)
			ofile.write(&(*buffer)[0], buffer->size());	// write the whole file

		if(! ofile.good())
			throw(file_io_error(std::string("error writing file : ") + file));
	}
}