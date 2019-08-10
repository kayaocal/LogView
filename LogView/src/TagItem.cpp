#include "TagItem.h"
#include <iostream>
#include <imgui_internal.h>

TagItem::TagItem(bool isActive, const ImVec4 & texCol, const ImVec4 & bgCol, int tagID)
{
	_tagID = tagID;
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

char* TagItem::GetBgColorHex()
{
	int red = _bgColor.x * 255;
	int green = _bgColor.y * 255;
	int blue = _bgColor.z * 255;

	snprintf(hexColor, sizeof hexColor, "%02x%02x%02x", red, green, blue);
	return hexColor;
}

char* TagItem::GetTextColorHex()
{
	int red = _textColor.x * 255;
	int green = _textColor.y * 255;
	int blue = _textColor.z * 255;
	snprintf(hexColor, sizeof hexColor, "%02x%02x%02x", red, green, blue); 
	return hexColor;
}

void TagItem::SetBGColorHex(char* hexCol)
{
	_bgColor.x = ((float)(HexToInt(hexCol[1]) + (HexToInt(hexCol[0]) * 16))) / 255;
	_bgColor.y = ((float)(HexToInt(hexCol[3]) + (HexToInt(hexCol[2]) * 16))) / 255;
	_bgColor.z = ((float)(HexToInt(hexCol[5]) + (HexToInt(hexCol[4]) * 16))) / 255;
}

void TagItem::SetTextColorHex(char* hexCol)
{
	_textColor.x = (float)(HexToInt(hexCol[1]) + HexToInt(hexCol[0]) * 16) / 255;
	_textColor.y = (float)(HexToInt(hexCol[3]) + HexToInt(hexCol[2]) * 16) / 255;
	_textColor.z = (float)(HexToInt(hexCol[5]) + HexToInt(hexCol[4]) * 16) / 255;
}

void TagItem::SetTag(char * str)
{
	strcpy(_textBuffer, str);
}

void TagItem::SetTagID(int tagID)
{
	_tagID = tagID;
}

int TagItem::GetTagID()
{
	return _tagID;
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
