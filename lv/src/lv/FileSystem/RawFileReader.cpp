#include <lv/FileSystem/RawFileReader.hpp>
#include <fstream>
#include <lv/Exception.hpp>

namespace lv
{
	void RawFileReader::fulfill(std::string const & file, BufferPtr buf)
	{
		std::ifstream infile(file.c_str(), std::ios_base::binary);
		if(! infile)
		{
			throw(file_io_error("error opening file: " + file));
		}

		// get the size of the file
		infile.seekg(0, std::ios_base::end);
		std::streamsize len = infile.tellg();
		if(len != 0)
		{
			// read the whole file
			infile.seekg(0, std::ios_base::beg);

			init_buffer(*buf, len);
			infile.read(buffer::data(buf), len);
		}
		else
		{
			buf->clear();
		}

		if(! infile.good())
		{
			throw(file_io_error("error reading file : " + file));
		}
	}
}
