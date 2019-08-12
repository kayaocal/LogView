#pragma once

#include <iostream>

#define MAX_FILE_NAME 200
#define MAX_FILE_TITLE 50
#define MAX_TAG_LENGTH 50
#define MAX_TAG_COUNT  32
#define MAX_OPEN_FILE_COUNT 20
#define FONT_SIZE 15
#define MAX_CLIPBOARD_COPY 1000

#define KEY_CODE_O 79
#define KEY_CODE_F3 114
#define KEY_CODE_ESC 27


static int HexToInt(char hex)
{
	if (hex >= 48 && hex < 58)
	{
		return hex - 48;
	}

	if (hex > 96 && hex < 103)
	{
		return hex - 87;
	}

	if (hex > 64 && hex < 71)
	{
		return hex - 55;
	}
}

static void ByteToChar(char* buffer, std::uintmax_t size)
{
	int kb = size / 1024;
	int mb = kb / 1024;

	if (mb > 0)
	{
		std::sprintf(buffer, "%d.%d mb", mb, kb % 1024);
	}
	else if (kb > 0)
	{
		std::sprintf(buffer, "%d.%d kb", kb, (size % 1024));
	}
	else
	{
		std::sprintf(buffer, "00.%d kb", size);
	}

}

static char* ColorToHex(char* hexColor, int size, int red, int green, int blue)
{
	snprintf(hexColor, size, "%02x%02x%02x", red, green, blue);
	return hexColor;
}

static void HexToColor(char* hexColor, float* red, float* green, float* blue)
{
	*red   = ((float)(HexToInt(hexColor[1]) + (HexToInt(hexColor[0]) * 16))) / 255;
	*green = ((float)(HexToInt(hexColor[3]) + (HexToInt(hexColor[2]) * 16))) / 255;
	*blue  = ((float)(HexToInt(hexColor[5]) + (HexToInt(hexColor[4]) * 16))) / 255;
}

static void SetColorArray(float* arr, float red, float green, float blue, float alpha)
{
	arr[0] = red;
	arr[1] = green;
	arr[2] = blue;
	arr[3] = alpha;
}