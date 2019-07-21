#pragma once
#include <Windows.h>
#include "General.h"
#include <cstring>
#include <comdef.h>
#include <iostream>
#include <fstream>

namespace LogView
{
	class LogFile
	{

	private:
		wchar_t _wFileName[MAX_FILE_NAME];
		wchar_t _wFileTitle[MAX_FILE_TITLE];

		char _fileTitle[MAX_FILE_TITLE];
		bool _isActive;
		bool _followTail;
		std::ifstream* _pStream;


	public:

		wchar_t* GetFileNameW();
		wchar_t* GetFileTitleW();
		char* GetFileTitleC();
		bool* IsActive();
		bool* IsFollowTailsActive();
		void SetFileTitleC(char* chr);

		void SerializeLogFile();

		LogFile(wchar_t* file_name, wchar_t* file_title);
		~LogFile();
	};

}
