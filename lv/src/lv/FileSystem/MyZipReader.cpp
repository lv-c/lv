#include <lv/FileSystem/MyZipReader.hpp>

#include <lv/Foreach.hpp>
#include <lv/MyZip.hpp>
#include <lv/lvlib.hpp>

#include <clocale>

namespace lv
{
	MyZipReader::MyZipReader(std::string const & password, std::string const & path, std::string const & postfix)
		: path_(path)
		, postfix_(postfix)
		, password_(password)
	{
		// appends '/' to path_ if necessary
		if(! path_.empty())
		{
			char last = path_[path_.size() - 1];
			if(last != '/' && last != '\\')
				path_.push_back('/');
		}
	}

	MyZipReader::~MyZipReader()
	{
		close();
	}

	void MyZipReader::close()
	{
		scoped_lock lock(mutex_);

		foreach(UnzipMap::value_type const & it, unzip_)
		{
			ZRESULT ret = it.second->close();
			BOOST_ASSERT(ret == ZR_OK);
		}
		unzip_.clear();
	}

	void MyZipReader::fulfill(std::string const & file, BufferPtr buf)
	{
		scoped_lock lock(mutex_);

		std::string zip_file = get_zip_file(file);
		if(zip_file.empty() || zip_file.size() + 1 >= file.size())	// + 1 : '\\' or '/'
			throw file_io_error("invalid file name:" + file);

		UnzipMap::iterator it = unzip_.find(zip_file);

		UnzipPtr uz;
		if(it == unzip_.end())
		{
			// open the zip file
			uz.reset(new MyUnzip());

			std::string zip_path = path_ + zip_file + postfix_;
			ZRESULT ret = uz->open(zip_path, password_);
			if(ret != ZR_OK)
				throw file_io_error("error opening zip file:" + zip_path);
			
			unzip_.insert(std::make_pair(zip_file, uz));
		}
		else
			uz = it->second;

		//
		int index;
		ZIPENTRY entry;
		ZRESULT	ret = uz->find_item(file.substr(zip_file.size() + 1), &index, &entry);

		if(ret == ZR_OK && entry.unc_size > 0)
		{
			init_buffer(*buf, static_cast<size_t>(entry.unc_size));
			ret = uz->unzip(index, *buf);
		}		

		// sometimes returns ZR_MORE while it's actually finished
		if(ret != ZR_OK && ret != ZR_MORE)
			throw file_io_error("error reading file:" + file);
	}


	std::string MyZipReader::get_zip_file(std::string const & file)
	{
		for(size_t i = 0; i < file.size(); ++i)
		{
			if(file[i] == '/' || file[i] == '\\')
			{
				std::string zip_file = file.substr(0, i);
				std::transform(zip_file.begin(), zip_file.end(), zip_file.begin(), ::tolower);
				return zip_file;
			}
		}

		return std::string();
	}
}