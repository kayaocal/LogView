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
	char _comboboxActiveTagsStr[MAX_TAG_COUNT * MAX_TAG_LENGTH];
	int _tagToEdit;
	int _activeTabIndex;
	int _currentLine;
	float _pureLogScrollY;
	bool _shouldGoToLine;
	char _copyClipboardData[MAX_CLIPBOARD_COPY];
	std::vector<LogView::LogFile*> _openedFiles;
	std::vector<TagItem*> _activeTags;
	LogView::LogFile* activeFile;
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
	int GetTagIndex(int tagID);
	int CalculateAvaibleTagID();
	void EditTag(bool isNew);
	void OnTagsRefreshed();
	void GoToLine(int file_index,int height, int lineNumber);
	int GetNextTagLineNumber(int file_index, int tagIndex);
	int GetPrevTagLineNumber(int file_index, int tagIndex);
	void GetPrevSearchLineNumber();
	void GetNextSearchLineNumber();
public:
	LogWindow();
	~LogWindow();

	ImFont* RobotoFont;


	



	void Render(float width, float height);
};

