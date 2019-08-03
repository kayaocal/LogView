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

void LogView::LogFile::ReadFile(std::vector<TagItem*>* tags)
{
	boost::filesystem::path p(_wFileName);
	boost::filesystem::fstream stream(p, boost::filesystem::ifstream::in);
	_buf.clear();
	LineOffsets.clear();
	LineTags.clear();
	_lineCount = 0;
	int l_offsets = 0;

	int tag_count = 0;

	if (tags != nullptr)
	{
		tag_count = tags->size();
	}

	for (std::string line; std::getline(stream, line); )
	{
		int tag_index = 0;
		int tag_id = 0;
		_buf.append(line.c_str());

		while (tag_index < tag_count)
		{
			if ((*tags)[tag_index]->Filter(line.c_str(), line.c_str() + line.length()))
			{
				tag_id = (*tags)[tag_index]->GetTagID();
				break;
			}
			tag_index++;
		}
			
		LineTags.push_back(tag_id);
		LineOffsets.push_back(l_offsets);
		l_offsets += line.length();
		_lineCount++;
	}

	stream.close();

}

///Returns 0 if file is not exist
///Returns 1 if file is not updated
///Returns 2 if file is updated
int LogView::LogFile::CheckFileStatus(bool shouldRead, std::vector<TagItem*>* tags)
{
	boost::filesystem::path p(_wFileName);
	if (boost::filesystem::exists(p)) {
		_isDeleted = false;

		uintmax_t size = boost::filesystem::file_size(p);
		if (_lastFileSize != size)
		{
			_lastFileSize = size;
			_isUpdated = true;
			if (shouldRead)
			{
				ReadFile(tags);
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
	_lastFileSize = 0;
	std::cout << "LogFile Converted fileTitle = "<< _fileTitle <<std::endl;
	
	std::cout << "File Size : " << file_size(file_name) << std::endl;


}

LogView::LogFile::~LogFile()
{
}
