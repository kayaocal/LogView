#pragma once
#include <imgui.h>
#include "General.h"

class TagItem
{

	ImVec4 _textColor;
	ImVec4 _bgColor;
	bool _isActive;
	char _textBuffer[MAX_TAG_LENGTH];

public:
	TagItem(bool isActive, const ImVec4& texCol, const ImVec4& bgCol);

	ImVec4& GetBgColor();
	ImVec4& GetTextColor();

	void SetTag(char* str);
	char* GetTag();
	void SetBgColor(float* col);
	void SetTextColor(float* col);
	void SetIsActive(bool isActive);
	void Toggle();
	bool IsActive();

	~TagItem();
};

