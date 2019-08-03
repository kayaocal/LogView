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
	int _activeTabIndex;
	std::vector<LogView::LogFile*> _openedFiles;
	std::vector<TagItem*> _activeTags;
	LogView::LogFile* activeFile;
	TagItem* _itemToEdit = nullptr;
	bool CanOpenSelectedFile(wchar_t* file_name);
	void DrawTabs();
	void AddToolTip(const char* chr);
	void DrawTagWorks(float width);
	void DrawPureLogs(float width, int tabId);
	void DrawTaggedLogs(float width, int tabId);
	int GetTagIndex(int tagID);
	int CalculateAvaibleTagID();
	void EditTag(bool isNew);
public:
	LogWindow();
	~LogWindow();

	ImFont* RobotoFont;


	



	void Render(float width, float height);
};

