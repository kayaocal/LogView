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

				if (ImGui::BeginTabItem(_openedFiles[n]->GetFileTitleC(), _openedFiles[n]->IsActive(), flags))
				{
					switch (_openedFiles[n]->CheckFileStatus(true))
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
					if (ImGui::Button("Next"))
					{

					}
					AddToolTip("Shortcut: F3");

					ImGui::SameLine();
					if (ImGui::Button("Previous"))
					{
					}
					AddToolTip("Shortcut: Shift + F3");

					ImGui::Separator();
					DrawPureLogs(width, n);
					DrawTaggedLogs(width, n);
					ImGui::EndTabItem();
				}
			}

			ImGui::EndTabBar();
		}

		ImGui::SameLine();
		ImGui::Spacing();
		
		DrawTagWorks(width);
		
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
		//TODO_ENES : Mesaj goster, Max sayýyý asmýs.
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

	int counter = _openedFiles[tabId]->GetLineCounter();
	ImGuiTextBuffer* buf = _openedFiles[tabId]->GetTextBuffer();
	const char* buf_start = buf->begin();
	const char* buf_end = buf->end();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (int i = 0; i < counter; i++)
	{
		const char* line_start = buf_start + _openedFiles[tabId]->LineOffsets[i];
		const char* line_end = (i + 1 < _openedFiles[tabId]->LineOffsets.Size) ? (buf_start + _openedFiles[tabId]->LineOffsets[i + 1] - 1) : buf_end;
		ImGui::PushID(i);
		static ImVec4 col;
		bool shouldPaint = false;
		int tagId = 0;
		for (int j = 0; j < _activeTags.size(); j++)
		{
			if (_activeTags[j]->Filter(line_start, line_end))
			{
				shouldPaint = true;
				tagId = j;
			}
		}
		if (shouldPaint)
		{
			ImGui::PushStyleColor(ImGuiCol_TextBG, _activeTags[tagId]->GetBgColor());
			ImGui::TextUnformattedWBg(line_start, line_end);
			ImGui::PopStyleColor(1);
		}
		else
		{
			ImGui::TextUnformatted(line_start, line_end);
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

void LogWindow::DrawTaggedLogs(float width, int tabId)
{

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

	for (int i = 0; i < tagCount; i++)
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

				//EditTag(_activeTags[i], false);
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
	for (int i = 0; i < tagCount; i++)
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
		TagItem* it = new TagItem(true, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		EditTag(it, true);
	}

	if (_tagToEdit >= 0)
	{
		ImGui::OpenPopup("UpdateTag");
		if (ImGui::BeginPopupContextItem("UpdateTag"))
		{
			EditTag(_activeTags[_tagToEdit], false);
		}
	}
}

void LogWindow::EditTag(TagItem * item, bool isNew)
{
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
		}
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
		for (int i = 0; i < _activeTags.size(); i++)
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

		delete(item);
		_tagToEdit = -1;
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();
}


