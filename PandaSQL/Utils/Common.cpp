
#include "stdafx.h"

#include "Common.h"

#include <sstream>

namespace PandaSQL
{

void StringToNumber(const std::string &inStr, uint32_t *o_num)
{
	*o_num = atoi(inStr.c_str());
}

void NumberToString(const uint32_t inNum, std::string *o_str)
{
	std::stringstream convert;

	convert << inNum;

	*o_str = convert.str();
}


}	// PandaSQL
