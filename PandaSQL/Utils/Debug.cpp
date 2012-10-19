#include "stdafx.h"

#include "Utils/Debug.h"

#include <iostream>

namespace PandaSQL
{

void DebugString(const char *str)
{
	char charStr[255];		
	sprintf_s(charStr, 255, "%s\n", str);
	
	std::cout << charStr ;
}

void DebugStringVerbose(const char *fileName, int32_t lineNum, const char *str)
{
	const char *nameStr = fileName;
	const char *nameOnly = strrchr(fileName, '\\');

	if (nameOnly)
	{
		nameStr = nameOnly + 1;
	}

	char charStr[255];		
	sprintf_s(charStr, 255, "%s (file: %s line: %d)\n", str, nameStr, lineNum);
	
	OutputDebugStringA(charStr);
}

}	// PandaSQL
