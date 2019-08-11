#pragma once
#include <imgui.h>
#include <vector> 
#include <iostream>
#include <Windows.h>
#include "General.h"
#include "LogFile.h"
#include "TagItem.h"
#include <yaml-cpp/yaml.h>

class LogWindow
{

private :
	ImGuiWindowFlags window_flags;
	OPENFILENAMEW _openFileName;
	
	bool p_open;
	bool _shouldGoToLine;
	wchar_t _fileNameBuffer[MAX_FILE_NAME];
	wchar_t _fileTitleBuffer[MAX_FILE_TITLE];
	char _comboboxActiveTagsStr[MAX_TAG_COUNT * MAX_TAG_LENGTH];
	char _copyClipboardData[MAX_CLIPBOARD_COPY];
	int _tagToEdit;
	int _activeTabIndex;
	int _currentLine;
	float _pureLogScrollY;
	char bufTagName[MAX_TAG_LENGTH];

	std::vector<LogView::LogFile*> _openedFiles;
	std::vector<TagItem*> _activeTags;

	TagItem* _searchTag;
	char _searchData[MAX_TAG_LENGTH];
	YAML::Node _appData;
	boost::filesystem::path _appDataPath;
	
	TagItem* _itemToEdit = nullptr;
	
	bool CanOpenSelectedFile(wchar_t* file_name);
	void DrawTabs();
	void AddToolTip(const char* chr);
	void DrawTagWorks(float width);
	void DrawPureLogs(float width, int tabId);
	void CalculateClipboardDataByLine(int tabId, int line);
	void CalculateClipboardDataBySelectedLines(int tabId);
	void CopyClipboardDataToClipboard();
	void DrawTaggedLogs(float width, int tabId);
	void EditTag(bool isNew);
	void OnTagsRefreshed();
	void GoToLine(int file_index,int height, int lineNumber);
	void ReadSavedData();

	int GetTagIndex(int tagID);
	int CalculateAvaibleTagID();
	int GetNextTagLineNumber(int file_index, int tagIndex);
	int GetPrevTagLineNumber(int file_index, int tagIndex);
	int GetNextSearchLineNumber(int file_index);
	int GetPrevSearchLineNumber(int file_index);

public:
	LogWindow();
	~LogWindow();

	void SaveAppData();

	void Render(float width, float height);
};

