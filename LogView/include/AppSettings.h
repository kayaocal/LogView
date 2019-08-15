#pragma once
#include <iostream>

struct AppSettings
{
	bool ShowLineNumbers;
	bool ShowTagIconOnPureLogs;
	float LineSpacingX;
	float LineSpacingY;
	int FontSize;
	char SearchBgColor[7];
	char SearchTextColor[7];

	AppSettings()
	{
		SetDefaults();
	}

	void SetDefaults()
	{
		ShowLineNumbers = true;
		ShowTagIconOnPureLogs = true;
		LineSpacingX = 4;
		LineSpacingY = 0.5f;
		FontSize = 0;
		std::strcpy(SearchTextColor, "ffffff");
		std::strcpy(SearchBgColor, "000000");
	}
};