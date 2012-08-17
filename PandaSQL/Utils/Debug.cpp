#include "stdafx.h"

#include "Debug.h"

namespace PandaSQL
{

void DebugString(const char *fileName, int32_t lineNum, const char *str)
{
	const char *nameStr = fileName;
	const char *nameOnly = strrchr(fileName, '\\');

	if (nameOnly)
	{
		nameStr = nameOnly + 1;
	}

	char errStr[255];		
	sprintf_s(errStr, 255, "%s (file: %s line: %d)\n", str, nameStr, lineNum);
	
	OutputDebugStringA(errStr);
}

}	// PandaSQL
