#pragma once
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