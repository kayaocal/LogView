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
					ImGui::BeginChild("scrolling", ImVec2(width - 25, 500), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
						ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar);
					
					int counter = _openedFiles[n]->GetLineCounter();
					ImGuiTextBuffer* buf = _openedFiles[n]->GetTextBuffer();
					const char* buf_start = buf->begin();
					const char* buf_end = buf->end();

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

					for (int i = 0; i < counter; i++)
					{
						const char* line_start = buf_start + _openedFiles[n]->LineOffsets[i];
						const char* line_end = (i + 1 < _openedFiles[n]->LineOffsets.Size) ? (buf_start + _openedFiles[n]->LineOffsets[i + 1] - 1) : buf_end;
						
						ImGui::TextUnformatted(line_start, line_end);
					}
					
					if (*_openedFiles[n]->IsFollowTailsActive())
						ImGui::SetScrollHereY(1.0f);

					ImGui::PopStyleVar();
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
			
			}

		
			ImGui::EndTabBar();
		}
		ImGui::Separator();
	}

	ImGui::SetWindowSize(ImVec2(width, height), 0);
	ImGui::SetItemDefaultFocus();

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


