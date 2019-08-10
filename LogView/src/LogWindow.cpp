#include "LogWindow.h"
#include <iostream>

LogWindow::LogWindow()
{
	LogWindow::window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoNav;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	window_flags |= ImGuiWindowFlags_MenuBar;
	_tagToEdit = -1;
	p_open = true;

		
	_appDataPath = boost::filesystem::temp_directory_path();
	
	_appDataPath.append(L"\\LogView");
	if (boost::filesystem::is_directory(_appDataPath) == false)
	{
		std::cout << "Directory not exist"<<std::endl;
		if (boost::filesystem::create_directory(_appDataPath))
		{
			std::cout << "Create directory success" << std::endl;
		}
		else
		{
			std::cout << "Create directory is not succesfull" << std::endl;

		}
	}

	_appDataPath.append(L"\\app.yaml");
	std::wcout << _appDataPath.c_str() << std::endl;
	if (boost::filesystem::exists(_appDataPath))
	{
		std::cout << "File Exist!" << std::endl;
		ReadSavedData();
	}
	else
	{
		std::cout << "File is not exist!" << std::endl;

		SaveAppData();
		
	}

	  

}

LogWindow::~LogWindow()
{
}

void LogWindow::Render(float width, float height)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);

	ImGui::Begin("Main", &p_open, window_flags);
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O")) 
			{
				std::cout << "Open menu button clicked -- " << std::endl;
				ZeroMemory(&_openFileName, sizeof(OPENFILENAMEW));
				_openFileName.lStructSize = sizeof(_openFileName);
				_openFileName.nFilterIndex = 1;
				_openFileName.nMaxFile = MAX_FILE_NAME;
				_openFileName.nMaxFileTitle = MAX_FILE_TITLE;
				_openFileName.lpstrFile = _fileNameBuffer;
				_openFileName.lpstrFileTitle = _fileTitleBuffer;
				_openFileName.lpstrFile[0] = '\0';
				_openFileName.lpstrFileTitle[0] = '\0';
				_openFileName.lpstrTitle = L"Select a File";
				_openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				_openFileName.lpstrFilter = L".Text Files\0*.txt";
				if (GetOpenFileNameW(&_openFileName))
				{
					std::wcout << "Can open requested file : " << _openFileName.lpstrFile << std::endl;
					if (CanOpenSelectedFile(_openFileName.lpstrFile))
					{
						std::cout << "-0-"<<std::endl;
						std::wcout << "Can open requested file title : " << _openFileName.lpstrFileTitle << std::endl;
						LogView::LogFile* file = new LogView::LogFile(_openFileName.lpstrFile, _openFileName.lpstrFileTitle);
						_openedFiles.push_back(file);
						
					}
					else
					{
						std::wcout << "Cannot open requested file : " << _openFileName.lpstrFile << std::endl;
					}
					
				}
				else
				{
					std::cout << "File open request failed : " << CommDlgExtendedError() << std::endl;
				}
			}

			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{

			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

		int opened_file_size = _openedFiles.size();
		static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
		tab_bar_flags |= ImGuiTabBarFlags_AutoSelectNewTabs;
		tab_bar_flags |= ImGuiTabBarFlags_FittingPolicyResizeDown;
		tab_bar_flags |= ImGuiTabBarFlags_FittingPolicyScroll;
		tab_bar_flags |= ImGuiTabBarFlags_FittingPolicyScroll;

		if (opened_file_size > 1)
		{
			tab_bar_flags |= ImGuiTabBarFlags_TabListPopupButton;
		}

		// Passing a bool* to BeginTabItem() is similar to passing one to Begin(): the underlying bool will be set to false when the tab is closed.
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			bool AutoScroll = true;
			bool ScrollToBottom = false;
			for (int n = 0; n < opened_file_size; n++) 
			{
				ImGuiTabItemFlags flags = 0;
				if (_openedFiles[n]->IsUpdated())
				{
					flags |= ImGuiTabItemFlags_FileUpdated;
				}

				if (_openedFiles[n]->IsDeleted())
				{
					flags |= ImGuiTabItemFlags_FileNotFound;
				}
			
				if (ImGui::BeginTabItem(_openedFiles[n]->GetFileNameC()))
				{
					_activeTabIndex = n;
					switch (_openedFiles[n]->CheckFileStatus(true, &_activeTags))
					{
					case 0:
						std::cout << "FILE IS NOT FOUND" << std::endl;
						break;
					case 1:
						//std::cout << "FILE IS NOT UPDATED" << std::endl;
						break;
					case 2:
						std::cout << "FILE IS UPDATED JUST YOUR INFO" << std::endl;
						break;
					default:
						break;
					}
					if (ImGui::Button("Change Tab Name"))
						ImGui::OpenPopup("ChangeTabNamePopup");

					AddToolTip("Click here to change tab's name");
					if (ImGui::BeginPopupContextItem("ChangeTabNamePopup"))
					{
						static char buf1[MAX_FILE_TITLE];
						if (ImGui::InputText("", buf1, MAX_FILE_TITLE, ImGuiInputTextFlags_EnterReturnsTrue))
						{
							_openedFiles[n]->SetFileTitleC(buf1);
							ImGui::CloseCurrentPopup();
						}
						AddToolTip("Give a name to this tab");
						ImGui::SameLine();
						if (ImGui::Button("SAVE"))
						{
							_openedFiles[n]->SetFileTitleC(buf1);
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					ImGui::Checkbox("Follow Tail", _openedFiles[n]->IsFollowTailsActive());
					ImGui::SameLine();
					ImGui::SetNextItemWidth(300);
					static char searchBuff[32];
					if (ImGui::InputText("Search", searchBuff, 32, ImGuiInputTextFlags_EnterReturnsTrue))
					{

					}
					AddToolTip("Shortcut: Ctrl + F");
				
					ImGui::SameLine();
					if (ImGui::ArrowButton("LeftBtn", ImGuiDir_Left))
					{
						
					}
					AddToolTip("Previos Search \n Shortcut: Shift + F3");
					
					ImGui::SameLine();
					if (ImGui::ArrowButton("RightBtn", ImGuiDir_Right))
					{

					}
					AddToolTip("Next Search \n Shortcut: F3");

					ImGui::SameLine();
					
					ImGui::SetNextItemWidth(150);

					static int item_current = 0;
			
					ImGui::Combo("Tags", &item_current, &(_comboboxActiveTagsStr[0]), (int)_activeTags.size());
					
				
					ImGui::SameLine();
					ImGui::ArrowButton("LeftBtn", ImGuiDir_Left);
					if (ImGui::IsItemHovered())
					{
						AddToolTip("Go To Previous Tag ");
						if (ImGui::IsMouseClicked(0))
						{
							std::cout << "Left button clicked" << std::endl;
							int lineNumber = GetPrevTagLineNumber(n, item_current);
							GoToLine(n, 400, lineNumber);
							
						}
					}

					ImGui::SameLine();
					ImGui::ArrowButton("RightBtn", ImGuiDir_Right);

					if (ImGui::IsItemHovered())
					{
						AddToolTip("Go To Next Tag ");
						if (ImGui::IsMouseClicked(0))
						{
							std::cout << "Right button clicked" << std::endl;
							int lineNumber = GetNextTagLineNumber(n, item_current);
							GoToLine(n, 400, lineNumber);
							_shouldGoToLine = true;
						}
					}
					
					ImGui::Separator();
					DrawPureLogs(width, n);
					ImGui::Separator();
					DrawTagWorks(width);
					ImGui::Separator();
					DrawTaggedLogs(width, n);
					ImGui::EndTabItem();
				}
			
}

			ImGui::EndTabBar();
		}

		ImGui::SameLine();
		ImGui::Spacing();
		
	}

	ImGui::SetWindowSize(ImVec2(width, height), 0);
	
	ImGui::SetItemDefaultFocus();

//	ImGui::ShowStyleEditor();
	ImGui::End();
}

bool LogWindow::CanOpenSelectedFile(wchar_t * file_name)
{
	int length = _openedFiles.size();

	if (length == MAX_OPEN_FILE_COUNT)
	{
		//TODO_ENES : Mesaj goster, Max say�y� asm�s.
		return false;
	}

	for (int i = 0; i < length; i++)
	{
		std::wcout << "File 1 : " << file_name<< " - File 2 : " << _openedFiles[i]->GetFileNameW() <<std::endl;
		if (std::wcscmp(file_name, _openedFiles[i]->GetFileNameW()) == 0)
		{
			return false;
		}
	}

	return true;
}

void LogWindow::DrawTabs()
{

}

void LogWindow::AddToolTip(const char * chr)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text(chr);
		ImGui::EndTooltip();
	}
}

void LogWindow::DrawPureLogs(float width, int tabId)
{
	ImGui::BeginChild("scrolling", ImVec2(width - 25, 400), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar);

	int counter = _openedFiles[tabId]->GetLineCount();
	ImGuiTextBuffer* buf = _openedFiles[tabId]->GetTextBuffer();
	const char* buf_start = buf->begin();
	const char* buf_end = buf->end();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (int i = 0; i < counter; i++)
	{
		const char* line_start = buf_start + _openedFiles[tabId]->LineOffsets[i];
		const char* line_end = (i + 1 < _openedFiles[tabId]->LineOffsets.Size) ? (buf_start + _openedFiles[tabId]->LineOffsets[i + 1] ) : buf_end;
		ImGui::PushID(i);
		static ImVec4 col;
		bool shouldPaint = false;
		int tagId = 0;
		
		if (_openedFiles[tabId]->LineTags[i] > 0)
		{
			int tagIndex = GetTagIndex(_openedFiles[tabId]->LineTags[i]);
			if (tagIndex >= 0 && _activeTags[tagIndex]->IsActive())
			{
				ImGui::PushStyleColor(ImGuiCol_TextBG, _activeTags[tagIndex]->GetBgColor());
				ImGui::PushStyleColor(ImGuiCol_Text, _activeTags[tagIndex]->GetTextColor());
				ImGui::SelectableTextUnformattedBG(line_start, line_end, &(_openedFiles[tabId]->LineSelections[i]));
				ImGui::PopStyleColor(2);
			}
			else
			{
				ImGui::SelectableTextUnformatted(line_start, line_end, &(_openedFiles[tabId]->LineSelections[i]));
			}

		}
		else
		{
			ImGui::SelectableTextUnformatted(line_start, line_end, &(_openedFiles[tabId]->LineSelections[i]));
		}

	}

	if (*_openedFiles[tabId]->IsFollowTailsActive())
	{
		ImGui::SetScrollHereY(1.0f);
	}
	else if(_shouldGoToLine == true)
	{
		
		std::cout << "_pureLogScrollY " << _pureLogScrollY << std::endl;
		std::cout << "Height " << ImGui::GetWindowHeight() << std::endl;
		ImGui::SetScrollY(_pureLogScrollY * 16 * _openedFiles[tabId]->GetLineCount());
		_shouldGoToLine = false;
	}
	
	static bool copyMenuActive = false;
	if (ImGui::IsWindowHovered())
	{
		static bool rightClickBuffer = false;
		if (ImGui::IsMouseDown(1) && !rightClickBuffer)
		{
			for (int i = 0; i < _openedFiles[tabId]->GetLineCount(); i++)
			{
				if (_openedFiles[tabId]->LineSelections[i] == true)
				{
					rightClickBuffer = true;
					copyMenuActive = true;
					std::cout << "Right Click" << std::endl;
					break;
				}
			}
		}
		else if(ImGui::IsMouseDown(1) == false)
		{
			rightClickBuffer = false;
		}
		
		if (copyMenuActive)
		{
			ImGui::OpenPopup("my_file_popupw");
		}
	}

	if (ImGui::BeginPopup("my_file_popupw"))
	{
		if (ImGui::IsMouseDown(0))
		{
			copyMenuActive = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::Button("Copy Selected Lines");
		
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0))
			{
				CalculateClipboardDataBySelectedLines(tabId);
				std::cout <<"clipboard copy value " << _copyClipboardData << std::endl;
				CopyClipboardDataToClipboard();
				
			}
		}
		

		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

void LogWindow::DrawTaggedLogs(float width, int tabId)
{
	ImGui::BeginChild("scrolling32", ImVec2(width - 25, 400), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar);

	int counter = _openedFiles[tabId]->GetLineCount();
	ImGuiTextBuffer* buf = _openedFiles[tabId]->GetTextBuffer();
	const char* buf_start = buf->begin();
	const char* buf_end = buf->end();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (int i = 0; i < counter; i++)
	{
		if (_openedFiles[tabId]->LineTags[i] > 0)
		{
			int tagIndex = GetTagIndex(_openedFiles[tabId]->LineTags[i]);
			if (tagIndex >= 0 && _activeTags[tagIndex]->IsActive())
			{
				const char* line_start = buf_start + _openedFiles[tabId]->LineOffsets[i];
				const char* line_end = (i + 1 < _openedFiles[tabId]->LineOffsets.Size) ? (buf_start + _openedFiles[tabId]->LineOffsets[i + 1] - 1) : buf_end;
				ImGui::PushID(i);
				static ImVec4 col;
				bool shouldPaint = false;
				int tagId = 0;
				ImGui::ArrowButton("UpToLine", ImGuiDir_Up);
				if (ImGui::IsItemHovered())
				{
					std::string str("Go To Line ");
					str += std::to_string(i);
					AddToolTip(str.c_str());
					if (ImGui::IsMouseClicked(0))
					{
						GoToLine(tabId, 400, i);
					}
				}

				ImGui::SameLine();
				ImGui::ArrowButton("Copy Line", ImGuiDir_Down);
				if (ImGui::IsItemHovered())
				{
					std::string str("Copy Line ");
					str += std::to_string(i);
					AddToolTip(str.c_str());
					if (ImGui::IsMouseClicked(0))
					{
						CalculateClipboardDataByLine(tabId, i);
						CopyClipboardDataToClipboard();
					}
				}

				ImGui::SameLine();
		
				ImGui::PushStyleColor(ImGuiCol_TextBG, _activeTags[tagIndex]->GetBgColor());
				ImGui::PushStyleColor(ImGuiCol_Text, _activeTags[tagIndex]->GetTextColor());
				ImGui::TextUnformattedWBg(line_start, line_end);
				ImGui::PopStyleColor(2);
			}
		}
	}

	if (*_openedFiles[tabId]->IsFollowTailsActive())
		ImGui::SetScrollHereY(1.0f);

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

int LogWindow::GetTagIndex(int tagID)
{
	for (size_t i = 0; i < _activeTags.size(); i++)
	{
		if (_activeTags[i]->GetTagID() == tagID)
		{
			return i;
		}
	}
	return -1;

}

int LogWindow::CalculateAvaibleTagID()
{
	int tags[MAX_TAG_COUNT] = { 0 };
	
	for (size_t i = 0; i < _activeTags.size(); i++)
	{
		std::cout << "tag : " << _activeTags[i]->GetTagID()<<std::endl;
		int number = _activeTags[i]->GetTagID();
		int count=0;
		while (number != 1)
		{
			number = number >> 1;
			count++;
		}
		tags[count] = 1;
	}

	for (int i = 1; i < MAX_TAG_COUNT; i++)
	{
		if (tags[i] == 0)
		{
			std::cout << "returned id : " << i << std::endl;
			return i;
		}
	}

	return -1;
}

void LogWindow::DrawTagWorks(float width)
{
	size_t tagCount = _activeTags.size();

	ImGui::BeginChild("activeTags", ImVec2(width / 2 - 50, 50), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::Text("Active Tags: ");
	ImGui::SameLine();

	int l_counter = 0;
	int active_counter = 0;
	int additionalLine = 1;
	for (size_t i = 0; i < tagCount; i++)
	{
		if (_activeTags[i]->IsActive())
		{
			active_counter++;
		}
	}
	int passiveCounter = tagCount - active_counter;

	for (size_t i = 0; i < tagCount; i++)
	{
		if (_activeTags[i]->IsActive() == false)
		{
			continue;
		}
		if (i > 0)
		{
			if (l_counter >= active_counter / 2 && additionalLine > 0)
			{
				additionalLine--;
			}
			else
			{
				ImGui::SameLine();
			}

		}
		l_counter++;

		ImGui::PushID(l_counter);
		ImGui::PushStyleColor(ImGuiCol_Button, _activeTags[i]->GetBgColor());
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _activeTags[i]->GetBgColor());
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, _activeTags[i]->GetBgColor());
		ImGui::PushStyleColor(ImGuiCol_Text, _activeTags[i]->GetTextColor());

		ImGui::Button(_activeTags[i]->GetTag());
		if (ImGui::IsItemHovered())
		{
			AddToolTip("Right Click to Edit");
			if (ImGui::IsMouseClicked(0))
			{
				_activeTags[i]->Toggle();
			}
			if (ImGui::IsMouseClicked(1))
			{
				_tagToEdit = i;
			}

			std::cout << std::endl;
		}

		ImGui::PopStyleColor(4);
		ImGui::PopID();
	}

	ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginChild("deactiveTags", ImVec2(width / 2 - 50, 50), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::Text("Disabled Tags: ");
	ImGui::SameLine();
	l_counter = 0;
	additionalLine = 1;
	for (size_t i = 0; i < tagCount; i++)
	{
		if (_activeTags[i]->IsActive() == true)
		{
			continue;
		}
		if (i > 0)
		{
			if (l_counter >= passiveCounter / 2 && additionalLine > 0)
			{
				additionalLine--;
			}
			else
			{
				ImGui::SameLine();
			}
		}
		l_counter++;

		ImGui::PushID(l_counter);
		ImGui::PushStyleColor(ImGuiCol_Button, _activeTags[i]->GetBgColor());
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _activeTags[i]->GetBgColor());
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, _activeTags[i]->GetBgColor());
		ImGui::PushStyleColor(ImGuiCol_Text, _activeTags[i]->GetTextColor());
		ImGui::Button(_activeTags[i]->GetTag());
		if (ImGui::IsItemHovered())
		{
			AddToolTip("Right Click to Edit");
			if (ImGui::IsMouseClicked(0))
			{
				_activeTags[i]->Toggle();
			}
			if (ImGui::IsMouseClicked(1))
			{
				_tagToEdit = i;
			}

			std::cout << std::endl;
		}

		ImGui::PopStyleColor(4);
		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::SameLine();

	if (ImGui::Button("Add Tag"))
	{
		if (_activeTags.size() < MAX_TAG_COUNT)
		{
			ImGui::OpenPopup("AddNewTag");
		}
		else
		{
			std::cout << "Error :: Tag over flow! " << std::endl;
			//ImGui::Message
		}
		
	}

	if (ImGui::BeginPopupContextItem("AddNewTag"))
	{
		if (tagCount < 30)
		{		
			if (_itemToEdit == nullptr)
			{
				int tTagID = CalculateAvaibleTagID();
				if (tTagID >= 0)
				{
					std::cout << "Tag ID = " << tTagID << std::endl;
					_itemToEdit = new TagItem(true, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (1 << (tTagID)));
				}

			}	

			EditTag(true);
			return;
		}
	}

	if (_tagToEdit >= 0)
	{
		ImGui::OpenPopup("UpdateTag");
		if (ImGui::BeginPopupContextItem("UpdateTag"))
		{
			_itemToEdit = _activeTags[_tagToEdit];
			EditTag(false);
		}
	}
}

void LogWindow::OnTagsRefreshed()
{
	memset(_comboboxActiveTagsStr, 0, sizeof(char)*MAX_TAG_COUNT*MAX_TAG_LENGTH);

	int lenght = _activeTags.size();
	int strIndex = 0;
	for (int i = 0; i < lenght; i++)
	{
		int len = std::strlen(_activeTags[i]->GetTag());
		std::strcpy(&(_comboboxActiveTagsStr[strIndex]), _activeTags[i]->GetTag());
		strIndex = strIndex + len + 1;
	}
}

void LogWindow::GoToLine(int fileIndex, int height, int lineNumber)
{
	_shouldGoToLine = true;
	_openedFiles[fileIndex]->SetFollowTail(false);

	int lineCount = _openedFiles[fileIndex]->GetLineCount();
	int lineInWindow = (int)(height / (FONT_SIZE + 1));
	if (lineNumber < lineInWindow)
	{
		_pureLogScrollY = 0.0f;
	}
	else if (lineNumber > lineCount - lineInWindow)
	{
		_pureLogScrollY = 1.0f;
	}
	else
	{
		_pureLogScrollY = (float)lineNumber / (float)(lineCount);
	}
	std::cout << "height" << height << std::endl;
	std::cout << "lineInWindow" << lineInWindow << std::endl;
	std::cout << "lineCount" << lineCount << std::endl;
	std::cout << "lineNumber" << lineNumber << std::endl;
	std::cout << "scroll value : " << _pureLogScrollY << std::endl;
	_currentLine = lineNumber;

}

int LogWindow::GetNextTagLineNumber(int fileIndex, int tagIndex)
{
	int tagID = _activeTags[tagIndex]->GetTagID();
	std::cout << "GetNextTagLineNumber\n TagID : " << tagID << std::endl;
	std::cout << "_currentLine : " << _currentLine << std::endl;
	for (int i = _currentLine+1; i < _openedFiles[fileIndex]->GetLineCount(); i++)
	{
		if (_openedFiles[fileIndex]->LineTags[i] == tagID)
		{
			std::cout << "Return : " << i << std::endl;
			return i;
		}
	}

	return _currentLine;
}

int LogWindow::GetPrevTagLineNumber(int fileIndex, int tagIndex)
{
	if (_currentLine <= 0)
	{
		return 0;
	}

	int tagID = _activeTags[tagIndex]->GetTagID();
	std::cout << "GetPrevTagLineNumber\n TagID : " << tagID << std::endl;
	for (int i = _currentLine - 1; i >= 0; i--)
	{
		if (_openedFiles[fileIndex]->LineTags[i] == tagID)
		{
			std::cout << "Return : " << i << std::endl;
			return i;
		}
	}

	return _currentLine;
}

void LogWindow::ReadSavedData()
{
	//std::string file;
	//boost::filesystem::load_string_file(_appDataPath, file);
	//std::cout << "FILE : " << file << std::endl;
	_appData = YAML::LoadFile(_appDataPath.generic_string());

	for (int i = 0; i < _appData["OF_names"].size(); i++)
	{
		std::wstring wc(_appData["OF_names"][i].as<std::string>().length(), L'\0');
		std::wstring wc2(_appData["OF_titles"][i].as<std::string>().length(), L'\0');
		mbstowcs(&wc[0], _appData["OF_names"][i].as<std::string>().c_str(), _appData["OF_names"][i].as<std::string>().length());
		mbstowcs(&wc2[0], _appData["OF_titles"][i].as<std::string>().c_str(), _appData["OF_titles"][i].as<std::string>().length());
		LogView::LogFile* file = new LogView::LogFile(&wc[0], &wc2[0]);
		file->SetFollowTail(_appData["OF_tail"][i].as<bool>());
		_openedFiles.push_back(file);
	}

	for (int i = 0; i < _appData["TAG_names"].size(); i++)
	{
		int tTagID = CalculateAvaibleTagID();
		if (tTagID >= 0)
		{
			bool isActive = _appData["TAG_is_active"][i].as<bool>();
			TagItem* item = new TagItem(isActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (1 << (tTagID)));
			item->SetBGColorHex((char*)_appData["TAG_bg_color"][i].as<std::string>().c_str());
			item->SetTextColorHex((char*)_appData["TAG_text_color"][i].as<std::string>().c_str());
			item->SetTag((char*)_appData["TAG_names"][i].as<std::string>().c_str());
			_activeTags.push_back(item);
		}
	}

}

void LogWindow::SaveAppData()
{
	_appData.reset();
	_appData["OF_namesW"].push_back((char*)_openedFiles[0]->GetFileNameW());
	if (_openedFiles.size() > 0)
	{
		for (int i = 0; i < _openedFiles.size(); i++)
		{
			_bstr_t b(_openedFiles[i]->GetFileNameW());
			const char* c = b;
			_appData["OF_names"].push_back(c);
			_appData["OF_titles"].push_back(_openedFiles[i]->GetFileTitleC());
			_appData["OF_tail"].push_back(*_openedFiles[i]->IsFollowTailsActive());
		}
	}
	
	if (_activeTags.size() > 0)
	{
		for (int i = 0; i < _activeTags.size(); i++)
		{
			_appData["TAG_names"].push_back(_activeTags[i]->GetTag());
			_appData["TAG_bg_color"].push_back(_activeTags[i]->GetBgColorHex());
			_appData["TAG_text_color"].push_back(_activeTags[i]->GetTextColorHex());
			_appData["TAG_is_active"].push_back(_activeTags[i]->IsActive());
				
		}
	}
	

	boost::filesystem::ofstream fout(_appDataPath);
	fout << _appData;
}

void LogWindow::EditTag(bool isNew)
{
	if (_itemToEdit == nullptr)
	{
		std::cout << "Error : 386"<<std::endl;
		return;
	}

	TagItem* item = _itemToEdit;
	ImVec4 col = item->GetBgColor();
	static float col_bg[4] = { col.x, col.y, col.z, col.z };
	ImGui::ColorEdit4("Background Color", col_bg, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_NoInputs);
	ImGui::SameLine();
	ImGui::Text("Background Color");
	col = item->GetTextColor();
	static float col_txt[4] = { col.x, col.y, col.z, col.z };
	ImGui::ColorEdit4("Text Color", col_txt, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_NoInputs);
	ImGui::SameLine();
	ImGui::Text("Text Color");

	static char bufTagName[MAX_TAG_LENGTH];
	ImGui::InputText("", bufTagName, MAX_TAG_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue);
	

	ImGui::SameLine();
	const char* add_tag = "ADD";
	const char* update_tag = "UPDATE";
	
	const char* btn_text = add_tag;
	if (isNew == false)
	{
		strcpy(bufTagName, item->GetTag());
		btn_text = update_tag;
	}

	if (ImGui::Button(btn_text))
	{
		if (strlen(bufTagName) > 2)
		{
			if (isNew)
			{
				_activeTags.push_back(item);
			}
			item->SetBgColor(col_bg);
			item->SetTextColor(col_txt);
			item->SetTag(bufTagName);
			_tagToEdit = -1;
			ImGui::CloseCurrentPopup();
			if (_openedFiles.size() > 0)
			{
				_openedFiles[_activeTabIndex]->ReadFile(&_activeTags);
			}
		}
		OnTagsRefreshed();
		_itemToEdit = nullptr;
	}

	ImGui::SameLine();

	const char* delete_tag = "DELETE";
	const char* cancel_tag = "CANCEL";

	btn_text = cancel_tag;
	if (isNew == false)
	{
		btn_text = delete_tag;
	}

	if (ImGui::Button(btn_text))
	{
		int index = -1;
		for (size_t i = 0; i < _activeTags.size(); i++)
		{
			if (_activeTags[i] == item)
			{
				index = i;
				break;
			}
		}

		if (index >= 0)
		{
			_activeTags.erase(_activeTags.begin() + index);
		}
		OnTagsRefreshed();
		_itemToEdit = nullptr;
		delete(item);
		_tagToEdit = -1;
		ImGui::CloseCurrentPopup();
	}
	
	ImGui::EndPopup();
}

void LogWindow::CalculateClipboardDataByLine(int tabId, int line)
{
	memset(_copyClipboardData, 0, sizeof(char) * MAX_CLIPBOARD_COPY);
	int l_lenght = 0;
	ImGuiTextBuffer* buf = _openedFiles[tabId]->GetTextBuffer();
	const char* buf_start = buf->begin();
	const char* buf_end = buf->end();

	const char* line_start = buf_start + _openedFiles[tabId]->LineOffsets[line];
	const char* line_end = (line + 1 < _openedFiles[tabId]->LineOffsets.Size) ? (buf_start + _openedFiles[tabId]->LineOffsets[line + 1]) : buf_end;
	const char* chrptr = line_start;
	while (l_lenght < MAX_CLIPBOARD_COPY && chrptr != line_end)
	{
		_copyClipboardData[l_lenght] = *chrptr;
		chrptr++;
		l_lenght++;
	}
}

void LogWindow::CalculateClipboardDataBySelectedLines(int tabId)
{
	memset(_copyClipboardData, 0, sizeof(char) * MAX_CLIPBOARD_COPY);
	int l_lenght = 0;
	ImGuiTextBuffer* buf = _openedFiles[tabId]->GetTextBuffer();
	const char* buf_start = buf->begin();
	const char* buf_end = buf->end();

	for (int i = 0; i < _openedFiles[tabId]->GetLineCount(); i++)
	{
		if (_openedFiles[tabId]->LineSelections[i] == true)
		{
			const char* line_start = buf_start + _openedFiles[tabId]->LineOffsets[i];
			const char* line_end = (i + 1 < _openedFiles[tabId]->LineOffsets.Size) ? (buf_start + _openedFiles[tabId]->LineOffsets[i + 1]) : buf_end;
			const char* chrptr = line_start;
			while (l_lenght < MAX_CLIPBOARD_COPY && chrptr != line_end)
			{
				_copyClipboardData[l_lenght] = *chrptr;
				chrptr++;
				l_lenght++;
			}
		}
		if (l_lenght >= MAX_CLIPBOARD_COPY)
		{
			break;
		}
	}
}

void LogWindow::CopyClipboardDataToClipboard()
{
	const size_t len = std::strlen(_copyClipboardData) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), _copyClipboardData, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}




