#include "stdafx.h"

#include "WinUtils.h"

#include "Debug.h"

namespace PandaSQL
{

std::wstring s2ws(const std::string& inStr)
{
	std::wstring result;

	int nChar;
	int inLength = (int)inStr.length() + 1;
	nChar = MultiByteToWideChar(CP_UTF8, 0, inStr.c_str(), inLength, NULL, 0);
	wchar_t* buf = new wchar_t[nChar];

	if (!buf)
	{
		PDASSERT(0);		
	}
	else
	{
		nChar = MultiByteToWideChar(CP_UTF8, 0, inStr.c_str(), inLength, buf, nChar);
		if (nChar == 0)
		{
			PDASSERT(0);
		}
		else
		{
			result.assign(buf);
		}
	}

	delete []buf;

	return result;
}

}	// PandaSQL

