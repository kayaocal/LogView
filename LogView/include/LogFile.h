#pragma once
#include <Windows.h>
#include "General.h"
#include <cstring>
#include <comdef.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <boost/filesystem.hpp>
#include <cstring>
#include <imgui.h>
#include "TagItem.h"

using namespace boost::filesystem;

namespace LogView
{
	class LogFile
	{

	private:
		wchar_t _wFileName[MAX_FILE_NAME];
		wchar_t _wFileTitle[MAX_FILE_TITLE];

		char* _fileBuffer;

		char _fileTitle[MAX_FILE_TITLE];
		bool _isActive;
		bool _followTail;
		std::ifstream* _pStream;
		std::uintmax_t _lastFileSize;
		ImGuiTextBuffer     _buf;
		ImGuiTextFilter     _filter;
		int _lineCount = 0;

		bool _isUpdated;
		bool _isDeleted;


	
	public:
		ImVector<int>  LineOffsets;
		ImVector<int>  LineTags;

		bool IsUpdated();
		bool IsDeleted();
		wchar_t* GetFileNameW();
		wchar_t* GetFileTitleW();
		char* GetFileTitleC();
		bool* IsActive();
		bool* IsFollowTailsActive();
		void SetFileTitleC(char* chr);

		void SerializeLogFile();
		void ReadFile(std::vector<TagItem*>* tags = nullptr);
		int CheckFileStatus(bool shouldRead, std::vector<TagItem*>* tags = nullptr);
		int GetLineCounter();
		ImGuiTextBuffer* GetTextBuffer();
		LogFile(wchar_t* file_name, wchar_t* file_title);
		~LogFile();
	};

}
