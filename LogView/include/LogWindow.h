#pragma once
#include <imgui.h>
#include "imgui/imgui_internal.h"
#include <vector> 
#include <iostream>
#include <Windows.h>
#include "General.h"
#include "LogFile.h"
#include "TagItem.h"
#include "AppSettings.h"
#include <yaml-cpp/yaml.h>

class LogWindow
{

private :
	ImGuiWindowFlags window_flags;
	OPENFILENAMEW _openFileName;
	
	AppSettings* _appSettings;

	bool selectionStarted = false;
	bool p_open;
	bool _shouldGoToLine;
	wchar_t _fileNameBuffer[MAX_FILE_NAME];
	wchar_t _fileTitleBuffer[MAX_FILE_TITLE];
	char _comboboxActiveTagsStr[MAX_TAG_COUNT * (MAX_TAG_LENGTH + 32)];
	char _copyClipboardData[MAX_CLIPBOARD_COPY];
	char searchBuff[MAX_TAG_LENGTH];
	int _tagToEdit;
	int _activeTabIndex;
	int _currentLine;
	float _pureLogScrollY;
	char bufTagName[MAX_TAG_LENGTH];
	float _tagColorBufferBg[4];
	float _tagColorBufferText[4];
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
	void DrawPureLogs(float width, float height, int tabId);
	void CalculateClipboardDataByLine(int tabId, int line);
	void CalculateClipboardDataBySelectedLines(int tabId);
	void CopyClipboardDataToClipboard();
	void DrawTaggedLogs(float width, float height, int tabId);
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
	void ClearSelectedLines();
	void ClearOtherSelectedLines(int selected);
	void CheckForIllegalSelectedLine();

public:
	float pureLogWindowHeight;
	float taggedLogWindowHeight;
	ImFont* _currentFont;
	LogWindow(AppSettings* app);
	~LogWindow();
	char tempString[256];
	char tempStr[32];
	char tempStrM[64];
	char tempStrX[128];
	char tempStrXL[256];

	void OpenFile();

	ImFont* FontRegular[2];
	ImFont* FontMedium[2];

	void SaveAppData();

	void Render(float width, float height);
};

