#include <lv/FileSystem/RawFileReader.hpp>
#include <fstream>
#include <lv/Exception.hpp>

namespace lv
{
	void RawFileReader::fulfill(std::string const & file, BufferPtr buffer)
	{
		std::ifstream infile(file.c_str(), std::ios::in | std::ios::binary);
		if(! infile)
			throw(file_io_error(std::string("error opening file: ") + file));

		// get the size of the file
		infile.seekg(0, std::ios::end);
		std::streamsize len = infile.tellg();
		if(len != 0)
		{
			// read the whole file
			infile.seekg(0, std::ios::beg);
			buffer->resize(len);
			infile.read(&(*buffer)[0], len);
		}
		else
			buffer->clear();

		if(! infile.good())
			throw(file_io_error(std::string("error reading file : ") + file));
	}
}