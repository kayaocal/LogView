#include "TagItem.h"
#include <iostream>
#include <imgui_internal.h>

TagItem::TagItem(bool isActive, const ImVec4 & texCol, const ImVec4 & bgCol)
{
	_textColor = texCol;
	_bgColor = bgCol;
	_isActive = isActive;
}

ImVec4 & TagItem::GetBgColor()
{
	return _bgColor;
}

ImVec4 & TagItem::GetTextColor()
{
	return _textColor;
}

void TagItem::SetTag(char * str)
{
	strcpy(_textBuffer, str);
}

char * TagItem::GetTag()
{
	return _textBuffer;
}

void TagItem::SetBgColor(float* col)
{
	_bgColor.x = col[0];
	_bgColor.y = col[1];
	_bgColor.z = col[2];
	_bgColor.w = 1;
}

void TagItem::SetTextColor(float* col)
{
	_textColor.x = col[0];
	_textColor.y = col[1];
	_textColor.z = col[2];
	_textColor.w = 1;
}

void TagItem::SetIsActive(bool isActive)
{
	_isActive = isActive;
}

void TagItem::Toggle()
{
	SetIsActive(!_isActive);
}

bool TagItem::IsActive()
{
	return _isActive;
}

bool TagItem::Filter(const char * haystack, const char * haystack_end)
{
	
	if (ImStristr(haystack, haystack_end, _textBuffer, _textBuffer + strlen(_textBuffer)))
		return true;

	return false;
}

TagItem::~TagItem()
{
}
