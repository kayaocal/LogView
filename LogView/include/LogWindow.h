#pragma once
#include <imgui.h>
#include <vector> 
#include <iostream>
#include <Windows.h>
#include "General.h"
#include "LogFile.h"
class LogWindow
{

private :
	ImGuiWindowFlags window_flags;
	bool p_open;
	OPENFILENAMEW _openFileName;
	wchar_t _fileNameBuffer[MAX_FILE_NAME];
	wchar_t _fileTitleBuffer[MAX_FILE_TITLE];

	std::vector<LogView::LogFile*> _openedFiles;
	LogView::LogFile* activeFile;

	bool CanOpenSelectedFile(wchar_t* file_name);
	void DrawTabs();
	void AddToolTip(const char* chr);
public:
	LogWindow();
	~LogWindow();


	



	void Render(float width, float height);
};

