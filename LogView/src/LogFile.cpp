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

LogView::LogFile::LogFile(wchar_t * file_name, wchar_t * file_title)
{
	std::memcpy(_wFileName, file_name, sizeof(wchar_t) * MAX_FILE_NAME);
	std::memcpy(_wFileTitle, file_title, sizeof(wchar_t) * MAX_FILE_TITLE);
	sprintf_s(_fileTitle, "%ws", _wFileTitle);
	_isActive = true;
	std::cout << "LogFile Converted fileTitle = "<< _fileTitle <<std::endl;
	
	if (_pStream != nullptr)
	{
		delete(_pStream);
	}

	_pStream = new std::ifstream(file_name);

}

LogView::LogFile::~LogFile()
{
}
