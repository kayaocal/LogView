#pragma once
#include <imgui.h>
#include <vector> 
#include <iostream>
#include <Windows.h>
#include "General.h"
#include "LogFile.h"
#include "TagItem.h"

class LogWindow
{

private :
	ImGuiWindowFlags window_flags;
	bool p_open;
	OPENFILENAMEW _openFileName;
	wchar_t _fileNameBuffer[MAX_FILE_NAME];
	wchar_t _fileTitleBuffer[MAX_FILE_TITLE];
	int _tagToEdit;
	std::vector<LogView::LogFile*> _openedFiles;
	std::vector<TagItem*> _activeTags;
	LogView::LogFile* activeFile;

	bool CanOpenSelectedFile(wchar_t* file_name);
	void DrawTabs();
	void AddToolTip(const char* chr);
	void EditTag(TagItem * item, bool isNew);
public:
	LogWindow();
	~LogWindow();

	ImFont* RobotoFont;


	



	void Render(float width, float height);
};

