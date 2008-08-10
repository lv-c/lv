#include <lv/FileSystem/IOTask.hpp>
#include <lv/FileSystem/IFileIO.hpp>

namespace lv
{
	void IOTask::operator ()()
	{
		file_io_->fulfill(file_, buffer_);
	}
}