#include <lv/FileSystem/MyZipReader.hpp>
#include <lv/FileSystem/RawFileReader.hpp>

#include <lv/MyZip.hpp>
#include <lv/lvlib.hpp>
#include <lv/Ensure.hpp>

#include <clocale>

namespace lv
{
	MyZipReader::MyZipReader(std::string const & password, std::string const & working_dir, std::string const & postfix, bool in_memory /* = false */)
		: IFileIO(working_dir)
		, postfix_(postfix)
		, password_(password)
		, in_memory_(in_memory)
	{
	}

	MyZipReader::~MyZipReader()
	{
		reset();
	}

	void MyZipReader::reset()
	{
		lock_guard lock(mutex_);

		for (auto const & v : unzip_)
		{
			ZRESULT ret = v.second->close();
			BOOST_ASSERT(ret == ZR_OK);
		}
		unzip_.clear();
	}

	void MyZipReader::fulfill(std::string const & file, BufferPtr buf)
	{
		lock_guard lock(mutex_);

		std::string inner_path;
		UnzipPtr uz = get_unzip(file, inner_path);

		//
		int index;
		ZIPENTRY entry;
		ZRESULT	ret = uz->find_item(inner_path, &index, &entry);

		std::error_code err;

		if (ret == ZR_OK && entry.unc_size > 0)
		{
			init_buffer(*buf, static_cast<size_t>(entry.unc_size));
			ret = uz->unzip(index, *buf);

			// sometimes returns ZR_MORE while it's actually finished
			if (ret != ZR_OK && ret != ZR_MORE)
			{
				err = std::make_error_code(std::errc::io_error);
			}
		}
		else
		{
			err = std::make_error_code(std::errc::no_such_file_or_directory);
		}

		LV_ENSURE(!err, std::system_error(err, "error reading file: " + file));
	}

	MyZipReader::UnzipPtr MyZipReader::get_unzip(std::string const & file, std::string & inner_path)
	{
		std::string zip_file = get_zip_file(file);

		// + 1 : '\\' or '/'
		LV_ENSURE(!zip_file.empty() && zip_file.size() + 1 < file.size(),
			std::system_error(std::make_error_code(std::errc::invalid_argument), "invalid file name: " + file));

		UnzipMap::iterator it = unzip_.find(zip_file);
		UnzipPtr uz;

		if (it == unzip_.end())
		{
			// open the zip file
			uz = std::make_shared<MyUnzip>();

			std::string zip_path = resolve(zip_file + postfix_);
			ZRESULT ret;

			if (in_memory_)
			{
				if (!raw_file_reader_)
				{
					raw_file_reader_ = std::make_shared<RawFileReader>();
				}

				BufferPtr file_buf = std::make_shared<Buffer>();
				raw_file_reader_->fulfill(zip_path, file_buf);

				ret = uz->open(file_buf, password_);
			}
			else
			{
				ret = uz->open(zip_path, password_);
			}

			LV_ENSURE(ret == ZR_OK, std::system_error(std::make_error_code(std::errc::io_error), "error opening pkt file: " + zip_path));

			unzip_.emplace(zip_file, uz);
		}
		else
		{
			uz = it->second;
		}

		inner_path = file.substr(zip_file.size() + 1);
		return uz;
	}

	bool MyZipReader::exist(std::string const & file)
	{
		lock_guard lock(mutex_);

		try
		{
			std::string inner_path;
			UnzipPtr uz = get_unzip(file, inner_path);

			int index;
			ZIPENTRY ze;

			return (uz->find_item(inner_path, &index, &ze) == ZR_OK);
		}
		catch (std::exception const &)
		{
			return false;
		}
	}

	std::string MyZipReader::get_zip_file(std::string const & file)
	{
		for (size_t i = 0; i < file.size(); ++i)
		{
			if (file[i] == '/' || file[i] == '\\')
			{
				std::string zip_file = file.substr(0, i);
				std::transform(zip_file.begin(), zip_file.end(), zip_file.begin(), ::tolower);
				return zip_file;
			}
		}

		return std::string();
	}
}