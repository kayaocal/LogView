#include "LogFile.h"


wchar_t * LogView::LogFile::GetFileNameW()
{
	return _wFileName;
}

wchar_t * LogView::LogFile::GetFileTitleW()
{
	return _wFileTitle;
}

char * LogView::LogFile::GetFileTitleC()
{
	return _fileTitle;
}

bool * LogView::LogFile::IsActive()
{
	return &_isActive;
}

bool * LogView::LogFile::IsFollowTailsActive()
{
	return &_followTail;
}

void LogView::LogFile::SetFileTitleC(char* chr)
{
	strcpy_s(_fileTitle, chr);
	std::cout << "New File Title : " <<_fileTitle<<std::endl;
}

void LogView::LogFile::SerializeLogFile()
{
	std::cout << "ERR SerializeLogFile is NOT implemented" << std::endl;
}

void LogView::LogFile::ReadFile()
{
	boost::filesystem::path p(_wFileName);
	boost::filesystem::fstream stream(p, boost::filesystem::ifstream::in);
	_buf.clear();
	int l_offsets = 0;
	for (std::string line; std::getline(stream, line); )
	{
		_buf.append(line.c_str());
		LineOffsets.push_back(l_offsets);
		std::cout << "line ofset : " << l_offsets <<std::endl;
		l_offsets += line.length();
		_lineCount++;
	}

	stream.close();

}


///Returns 0 if file is not exist
///Returns 1 if file is not updated
///Returns 2 if file is updated
int LogView::LogFile::CheckFileStatus(bool shouldRead)
{
	boost::filesystem::path p(_wFileName);
	if (boost::filesystem::exists(p)) {
		_isDeleted = false;

		std::time_t t = boost::filesystem::last_write_time(p);

		if (t != _lastWriteTime)
		{
			_lastWriteTime = t;
			_isUpdated = true;
			if (shouldRead)
			{
				ReadFile();
			}

			return 2;
		}

		return 1;
	}
	
	_isDeleted = true;
	_isUpdated = false;
	return 0;
}

int LogView::LogFile::GetLineCounter()
{
	return _lineCount;
}

ImGuiTextBuffer * LogView::LogFile::GetTextBuffer()
{
	return &_buf;
}


bool LogView::LogFile::IsUpdated()
{
	return _isUpdated;
}

bool LogView::LogFile::IsDeleted()
{
	return _isDeleted;
}

LogView::LogFile::LogFile(wchar_t * file_name, wchar_t * file_title)
{
	std::memcpy(_wFileName, file_name, sizeof(wchar_t) * MAX_FILE_NAME);
	std::memcpy(_wFileTitle, file_title, sizeof(wchar_t) * MAX_FILE_TITLE);
	sprintf_s(_fileTitle, "%ws", _wFileTitle);
	_isActive = true;
	std::cout << "LogFile Converted fileTitle = "<< _fileTitle <<std::endl;
	
	std::cout << "File Size : " << file_size(file_name) << std::endl;


}

LogView::LogFile::~LogFile()
{
}
